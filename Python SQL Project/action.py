import repo_cli.utils.config

from persistence import *

import sys

def main(args):
    inputfilename: str = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            product_id, quantity, quantity_current, split_line = analyze_line(line)
            quantity_current = quantity_current[0][0]
            if quantity > 0:
                perform_action(product_id, quantity, quantity_current, split_line)
            elif quantity < 0 and quantity_current >= -quantity:
                perform_action(product_id, quantity, quantity_current, split_line)


def analyze_line(line):
    split_line = line.strip().split(", ")
    product_id = int(split_line[0])
    quantity = int(split_line[1])
    quantity_current = repo.execute_command("SELECT quantity FROM products WHERE id = ?", (product_id,))
    return product_id, quantity, quantity_current, split_line


def perform_action(product_id, quantity, quantity_current, splittedline):
    final_quantity = quantity + quantity_current
    repo.execute_command("UPDATE products SET quantity = ? WHERE id = ?", (final_quantity, product_id))
    repo.execute_command("INSERT INTO activities VALUES(?,?,?,?)",
                         (product_id, quantity, int(splittedline[2]), splittedline[3]))


if __name__ == '__main__':
    main(sys.argv)
