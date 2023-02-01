package bguspl.set.ex;

import bguspl.set.Env;

import java.util.*;

/**
 * This class manages the players' threads and data
 *
 * @inv id >= 0
 * @inv score >= 0
 */
public class Player implements Runnable {

    /**
     * The game environment object.
     */
    private final Env env;

    /**
     * Game entities.
     */
    private final Table table;

    /**
     * The id of the player (starting from 0).
     */
    public final int id;

    /**
     * The thread representing the current player.
     */
    private Thread playerThread;

    /**
     * The thread of the AI (computer) player (an additional thread used to generate key presses).
     */
    private Thread aiThread;

    /**
     * True iff the player is human (not a computer player).
     */
    private final boolean human;

    /**
     * True iff game should be terminated due to an external event.
     */
    private volatile boolean terminate;

    /**
     * The current score of the player.
     */
    private int score;

    public Queue<Integer> queueKeyPressed = new LinkedList<>();
    public boolean fullQueue;
    public boolean point;
    public boolean penalty;


    /**
     * The class constructor.
     *
     * @param env    - the environment object.
     * @param dealer - the dealer object.
     * @param table  - the table object.
     * @param id     - the id of the player.
     * @param human  - true iff the player is a human player (i.e. input is provided manually, via the keyboard).
     */
    public Player(Env env, Dealer dealer, Table table, int id, boolean human) {
        this.env = env;
        this.table = table;
        this.id = id;
        this.human = human;
        this.fullQueue = false;
        this.point = false;
        this.penalty = false;

    }

    /**
     * The main player thread of each player starts here (main loop for the player thread).
     */
    @Override
    public void run() {
        Thread.currentThread();
        System.out.printf("Info: Thread %s starting.%n", Thread.currentThread().getName());
        if (!human) createArtificialIntelligence();

        while (!terminate) {
            if (penalty) {
                penalty();
                penalty = false;
                if (!human) {
                    aiThread.interrupt();
                }
            }
            if (point) {
                point();
                point = false;
                if (!human) {
                    aiThread.interrupt();
                }
            }
        }
        if (!human) try {
            aiThread.join();
        } catch (InterruptedException ignored) {
        }
        System.out.printf("Info: Thread %s terminated.%n", Thread.currentThread().getName());
    }

    /**
     * Creates an additional thread for an AI (computer) player. The main loop of this thread repeatedly generates
     * key presses. If the queue of key presses is full, the thread waits until it is not full.
     */
    private void createArtificialIntelligence() {
        // note: this is a very very smart AI (!)
        aiThread = new Thread(() -> {
            System.out.printf("Info: Thread %s starting.%n", Thread.currentThread().getName());
            while (!terminate) {
                // TODO implement player key press simulator
                if(!fullQueue && table.allCardsOnTable && !table.changeCards) {
                    Random random = new Random();
                    keyPressed(random.nextInt(env.config.tableSize));
                }

            }
            System.out.printf("Info: Thread %s terminated.%n", Thread.currentThread().getName());
        }, "computer-" + id);
        aiThread.start();
    }

    /**
     * Called when the game should be terminated due to an external event.
     */
    public void terminate() {
        // TODO implement
        terminate = true;
    }

    /**
     * This method is called when a key is pressed.
     *
     * @param slot - the slot corresponding to the key pressed.
     */
    public void keyPressed(int slot) {
        if (table.slotToCard[slot] != null && !fullQueue && !penalty && !point) {
            boolean remove = false;
            if (queueKeyPressed.size() == 0) { // no token placed yet
                queueKeyPressed.add(slot);
                table.placeToken(id, slot);
            } else {//check if there is already a token in the slot
                for (int slotInQueue : queueKeyPressed) {
                    if (slotInQueue == slot) {
                        remove = true;
                    }
                }
                if (remove) {
                    table.removeToken(id, slot);
                    queueKeyPressed.remove(slot);
                }

                if (!remove) {
                    queueKeyPressed.add(slot);
                    table.placeToken(id, slot);
                    if (queueKeyPressed.size() == 3) {
                        //implement sync for checcking set
                        fullQueue = true;
                        boolean inserted = false;
                        while (!inserted && fullQueue) {
                            inserted = table.playerToCheck.compareAndSet(-1, id);
                        }
                    }
                }
            }
        }
        else if (fullQueue && !penalty && !point) {
            int size = queueKeyPressed.size();
            int count = 0;
            while (count < size) {
               int slotInQueue = queueKeyPressed.poll();
                    if (slotInQueue == slot) {
                        table.removeToken(id, slot);
                        fullQueue = false;
                    }
                    else {
                        queueKeyPressed.add(slotInQueue);
                    }
                    count++;
            }
        }
    }

    /**
     * Award a point to a player and perform other related actions.
     *
     * @post - the player's score is increased by 1.
     * @post - the player's score is updated in the ui.
     */
    public void point() {
        // TODO implement
        this.score++;
        int ignored = table.countCards(); // this part is just for demonstration in the unit tests
        env.ui.setScore(id, score);
        long endPoint = System.currentTimeMillis() + env.config.pointFreezeMillis;
        while (endPoint - System.currentTimeMillis() >= 0) {
            env.ui.setFreeze(id, endPoint - System.currentTimeMillis());
        }
        env.ui.setFreeze(id, endPoint - System.currentTimeMillis());
    }

    /**
     * Penalize a player and perform other related actions.
     */
    public void penalty() {
        long endPenalty = System.currentTimeMillis() + env.config.penaltyFreezeMillis;
        while (endPenalty - System.currentTimeMillis() >= 0) {
            env.ui.setFreeze(id, endPenalty - System.currentTimeMillis());
            try{playerThread.sleep(100);} catch(InterruptedException e){

            } 
        }
        env.ui.setFreeze(id, endPenalty - System.currentTimeMillis());
        if(!human){
            cleanQueue();
        }
    }

    public int getScore() {
        return score;
    }

    public void cleanQueue() {
        Iterator<Integer> iter = queueKeyPressed.iterator();
        while (iter.hasNext()) {
            table.removeToken(id, iter.next());
        }
        queueKeyPressed.clear();
        fullQueue = false;
    }
}