from persistence import *
import sys
import os

def add_branche(splittedline):
    repo.execute_command("INSERT INTO branches VALUES(?,?,?)",
                         (int(splittedline[0]), splittedline[1], int(splittedline[2])))


def add_supplier(splittedline):
    repo.execute_command("INSERT INTO suppliers VALUES(?,?,?)",
                         (int(splittedline[0]), splittedline[1], splittedline[2]))


def add_product(splittedline):
    repo.execute_command("INSERT INTO products VALUES(?,?,?,?)",
                         (int(splittedline[0]), splittedline[1], float(splittedline[2]), int(splittedline[3])))


def add_employee(splittedline):
    repo.execute_command("INSERT INTO employees VALUES(?,?,?,?)",
                         (int(splittedline[0]), splittedline[1], float(splittedline[2]), int(splittedline[3])))


adders = {"B": add_branche,
          "S": add_supplier,
          "P": add_product,
          "E": add_employee}


def main(args):
    inputfilename = args[1]
    # delete the database file if it exists
    repo._close()
    # uncomment if needed
    if os.path.isfile("bgumart.db"):
        os.remove("bgumart.db")
    repo.__init__()
    repo.create_tables()
    with open(inputfilename, 'r') as input_file:
        for line in input_file:
            split_line = line.strip().split(',')
            adders[split_line[0]](split_line[1:])


if __name__ == '__main__':
    main(sys.argv)
