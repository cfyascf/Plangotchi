import pyodbc

server = 'CT-C-0013F\SQLEXPRESS'
database = 'yasminteste'
driver= '{ODBC Driver 17 for SQL Server}'
# username = ''
# password = ''
conn= pyodbc.connect('DRIVER='+driver+';SERVER='+server+';PORT=1433;DATABASE='+database+';integrated security=True; MutipleActiveResultSets=True;')
cursor = cnxn.cursor()

def select_data(list, index):
    cursor.execute(f"SELECT sensor FROM dbo.Sensor")

    for row in cursor.fetchall():
        list.append(row[index])

    print("Data recovered with success!")

select_data()