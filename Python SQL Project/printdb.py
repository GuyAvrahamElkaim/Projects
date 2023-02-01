from persistence import *

def main():
    # TODO: implement
    activities = repo.execute_command("SELECT * FROM activities ORDER BY date,product_id", ())
    print("Activities")
    for element in activities:
        decoded_element = [item.decode() if isinstance(item, bytes) else item for item in element]
        print(tuple(decoded_element))
    print()

    tables = ["Branches", "Employees", "Products", "Suppliers"]
    for table in tables:
        table_content = repo.execute_command(f"SELECT * FROM {table} ORDER BY id", ())
        print(table)
        for element in table_content:
            decoded_element = [item.decode() if isinstance(item, bytes) else item for item in element]
            print(tuple(decoded_element))
        print()


    qstr1 = """
    SELECT  employees.name,
            employees.salary,
            branches.location,
            CASE WHEN SUB1.total_sales_income IS NULL THEN 0 ELSE SUB1.total_sales_income END AS total_sales_income
    FROM employees  JOIN branches
    ON employees.branche = branches.id
    LEFT JOIN
    (
    SELECT activities.activator_id,        
    activities.quantity * -1 * products.price AS total_sales_income 
    FROM activities JOIN products 
    ON activities.quantity < 0 AND activities.product_id = products.id 
    GROUP BY activities.activator_id
    )SUB1
    ON employees.id = SUB1.activator_id
    ORDER BY employees.name"""

    summary_employees = repo.execute_command(qstr1, ())
    print("Employees report")
    for element in summary_employees:
        for in_element in element:
            if isinstance(in_element, bytes):
                in_element = in_element.decode()
            print(in_element, end=" ")
        print()
    print()

    qstr2 = """ 
    SELECT activities.date,
           products.description,
           activities.quantity,
           employees.name,
          suppliers.name
    FROM  activities JOIN products
    ON activities.product_id = products.id
    LEFT JOIN employees
    ON activities.activator_id = employees.id
    LEFT JOIN suppliers
    ON activities.activator_id = suppliers.id
    ORDER BY activities.date
    """
    activitie_report = repo.execute_command(qstr2,())
    print("Activities report")
    for element in activitie_report:
        decoded_element = [item.decode() if isinstance(item, bytes) else item for item in element]
        print(tuple(decoded_element))

if __name__ == '__main__':
    main()
