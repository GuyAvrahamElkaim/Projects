package bguspl.set.ex;

import bguspl.set.Env;

import java.util.*;
import java.util.logging.Level;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * This class manages the dealer's threads and data
 */
public class Dealer implements Runnable {

    /**
     * The game environment object.
     */
    private final Env env;

    /**
     * Game entities.
     */
    private final Table table;
    private final Player[] players;

    /**
     * The list of card ids that are left in the dealer's deck.
     */
    private final List<Integer> deck;

    /**
     * True iff game should be terminated due to an external event.
     */
    private volatile boolean terminate;
    private Thread dealerThread;
    /**
     * The time when the dealer needs to reshuffle the deck due to turn timeout.
     */
    private long reshuffleTime = Long.MAX_VALUE;

    //added
    private long startTime;
    public int[] checkIfSet;
    //public boolean changeCards;

    public Dealer(Env env, Table table, Player[] players) {
        this.env = env;
        this.table = table;
        this.players = players;
        deck = IntStream.range(0, env.config.deckSize).boxed().collect(Collectors.toList());
        startTime = System.currentTimeMillis();
        //this.changeCards = false;
        this.checkIfSet = new int[3];
    }

    /**
     * The dealer thread starts here (main loop for the dealer thread).
     */
    @Override
    public void run() {
        System.out.printf("Info: Thread %s starting.%n", Thread.currentThread().getName());
        dealerThread = Thread.currentThread();
        for (Player player : players) {
            Thread t1 = new Thread(player, env.config.playerNames[player.id]);
            t1.start();
        }
        while (!shouldFinish()) {
            Collections.shuffle(deck);
            placeAllCardsOnTable();
            timerLoop();
            updateTimerDisplay(false);
            removeAllCardsFromTable();
        }
        announceWinners();
        System.out.printf("Info: Thread %s terminated.%n", Thread.currentThread().getName());
    }

    /**
     * The inner loop of the dealer thread that runs as long as the countdown did not time out.
     */
    private void timerLoop() {
        if (env.config.turnTimeoutMillis > 0) {
            reshuffleTime = System.currentTimeMillis() + env.config.turnTimeoutMillis;
        }
        while (!terminate && System.currentTimeMillis() < reshuffleTime) {
            sleepUntilWokenOrTimeout();
            updateTimerDisplay(false);
            checkSet();
            removeCardsFromTable();
            placeCardsOnTable();
        }
    }

    /**
     * Called when the game should be terminated due to an external event.
     */
    public void terminate() {
        // TODO implement
        for (Player player : players) {
            player.terminate();
        }
    }

    /**
     * Check if the game should be terminated or the game end conditions are met.
     *
     * @return true iff the game should be finished.
     */
    private boolean shouldFinish() {
        return terminate || env.util.findSets(deck, 1).size() == 0;
    }

    /**
     * Checks if any cards should be removed from the table and returns them to the deck.
     */
    private void removeCardsFromTable() {
        if (table.changeCards) {
            for (int i = 0; i < checkIfSet.length; i++) {
                table.removeCard(checkIfSet[i]);
            }
        }
    }

    /**
     * Check if any cards can be removed from the deck and placed on the table.
     */
    private void placeCardsOnTable() {
        while (table.changeCards) {

            for (int i = 0; i < checkIfSet.length && deck.size() > 0; i++) {
                int card = deck.remove(0); // case no card in deck IMPLEMENT
                table.placeCard(card, checkIfSet[i]);
            }

            if (env.config.turnTimeoutMillis > 0) {
                updateTimerDisplay(true);
            }
            else if(env.config.turnTimeoutMillis <= 0){
                if(!table.thereIsASetOnTheTable()){
                    removeAllCardsFromTable();
                    boolean isSet = false;
                    if(!shouldFinish()){
                        while (!isSet) {
                            placeAllCardsOnTable();
                            isSet = table.thereIsASetOnTheTable();
                            if (!isSet) {
                                removeAllCardsFromTable();
                            }
                        }
                    }
                    else{
                        terminate=true;
                    }
                }
            }
            table.changeCards = false;
        }
    }

    /**
     * Sleep for a fixed amount of time or until the thread is awakened for some purpose.
     */
    private void sleepUntilWokenOrTimeout() {
        // TODO implement
        synchronized (this) {
            if (reshuffleTime - System.currentTimeMillis() > env.config.turnTimeoutWarningMillis) {
                try {
                    dealerThread.sleep(100);
                } catch (InterruptedException e) {
                }
            } else {
                try {
                    dealerThread.sleep(10);
                } catch (InterruptedException e) {
                }
            }
        }
    }

    /**
     * Reset and/or update the countdown and the countdown display.
     */
    private void updateTimerDisplay(boolean reset) {
        if ((reset)&&env.config.turnTimeoutMillis > 0) {
            reshuffleTime = System.currentTimeMillis() + env.config.turnTimeoutMillis;
        }
        if (env.config.turnTimeoutMillis > 0) {
            // why don't we use the boolean reset
            long timeLeft = reshuffleTime - System.currentTimeMillis();
            boolean warn = timeLeft < env.config.turnTimeoutWarningMillis;
            if(timeLeft< 0){
                env.ui.setCountdown(0, warn);
            }
            else{
                env.ui.setCountdown(timeLeft, warn);
            }
        } else if(env.config.turnTimeoutMillis == 0) {
            long timePassed = System.currentTimeMillis() - startTime;
            env.ui.setCountdown(timePassed, false);
        }

    }

    /**
     * Returns all the cards from the table to the deck.
     */
    private void removeAllCardsFromTable() {
        // TODO implement
        for (int i = 0; i < env.config.tableSize; i++) {
            if (table.slotToCard[i] != null) {
                deck.add(table.slotToCard[i]);
                table.removeCard(i);
            }
        }
        for (int i = 0; i < players.length; i++) { // empty the queue after 60 seconds
            players[i].cleanQueue();
        }
        if (env.util.findSets(deck, 1).size() == 0) {
            terminate();
        }
        table.allCardsOnTable = false;
    }

    private void placeAllCardsOnTable() {
        for (int i = 0; i < env.config.tableSize && deck.size() > 0; i++) {
            int card = deck.remove(0);
            table.placeCard(card, i);
        }
        table.allCardsOnTable = true;
    }

    /**
     * Check who is/are the winner/s and displays them.
     */
    private void announceWinners() {
        int winnerId = 0;   // who has the max score
        for (int i = 0; i < players.length; i++) {
            if (players[i].getScore() > players[winnerId].getScore()) {
                winnerId = i;
            }
        }
        int numberOfWinners = 0;    // how many has the max score
        for (int i = 0; i < players.length; i++) {
            if (players[i].getScore() == players[winnerId].getScore()) {
                numberOfWinners++;
            }
        }
        int index = 0;
        int[] winners = new int[numberOfWinners]; // array of winners
        for (int i = 0; i < players.length; i++) {
            if (players[i].getScore() == players[winnerId].getScore()) {
                winners[index] = players[i].id;
                index++;
            }
        }
        env.ui.announceWinner(winners);
    }

    public void checkSet() {

        int player = table.playerToCheck.get();
        if (player != -1 && players[player].fullQueue) {
            int[] arrayOfSlots = queueToArray(players[player].queueKeyPressed);
            int[] threeCards = slotsToCardsInArray(arrayOfSlots);

            //if (!nullInArray) {
                if (env.util.testSet(threeCards)) { // It's a SET!
                    // resetting the timer
                    updateTimerDisplay(true);
                    // increment the player score
                    env.ui.setScore(player, players[player].getScore() + 1);
                    //true to activate point
                    players[player].point = true;
                    // remove those cards from table
                    checkIfSet = arrayOfSlots;
                    table.changeCards = true;
                    for (Player playerQueue :
                            players) {
                        for (int slotToRemove : arrayOfSlots) {
                            // checks for the same pressed keys
                            Iterator<Integer> iter = playerQueue.queueKeyPressed.iterator();
                            boolean remove = false;
                            while (iter.hasNext() && !remove) {
                                int slotInQueue = iter.next();
                                if (slotInQueue == slotToRemove) {
                                    playerQueue.queueKeyPressed.remove(slotToRemove);
                                    remove = true;
                                    playerQueue.fullQueue = false;
                                }
                            }

                        }
                    }

                }
                //if not set
                else {
                    //System.out.println("player " + player + " pressed wrong set");
                    players[player].penalty = true;
                }
//            }else{
//                System.out.println("player " + player + " null in array");
//                players[player].penalty = true;
//            }
        }
        //if player doesnt have a full queue we do nothing and wait for the next player to have a full queue
        //we change to -1 only after we have checked the set and remove relevant cards from players queues if needed
        //after changing the queues the fullQueue =false
        table.playerToCheck.set(-1);

    }

    public int[] queueToArray(Queue<Integer> queue){
        int[] arrayOfSlots = new int[3];
        int index = 0;
        for (Integer slot :
                queue) {
            arrayOfSlots[index] = slot;
            index++;
        }
        return arrayOfSlots;
    }
    public int[] slotsToCardsInArray(int[] arrayOfSlots){
        int[] threeCards = new int[3];
        for (int i = 0; i < threeCards.length; i++) { // slot to cards in the array
            try {
                threeCards[i] = table.slotToCard[arrayOfSlots[i]];
            } catch (Exception e) {
            }
        }
        return threeCards;
    }


}