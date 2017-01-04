# shoppingCart
This project is shopping cart using C and Mysql. You should have GCC compiler and mysql installed in your computer.

You should have basic knowledge on mysql, C.

-------------------------------------------------------------------
1st: Need to upload the database. (database name: test_db)

To restore a database dump created with mysqldump, you simply have to redirect the file into MySQL again.
We need to create a blank database to house the imported data. First, log into MySQL by typing:

mysql -u username -p

Create a new database which will hold all of the data from the data dump and then exit out of the MySQL prompt:

CREATE DATABASE database_name;
exit

Next, we can redirect the dump file into our newly created database by issuing the following command:

mysql -u username -p database_name < backup_name.sql

Your information should now be restored to the database you've created.

----------------------------------------------------------------------
To confirm use the following command
show databases;
----------------------------------------------------------------------
2nd: How to run the program
  1. Start mysql server
  sudo service mysql start
  2. Compile the program. (database_c.c is the file name)
  gcc -o output $(mysql_config --cflags) database_c.c $(mysql_config --libs)
  3. run the program
  ./output
----------------------------------------------------------------------
After running the program use ID 1 or 2. I have already created user with these ID. you need to do it manually.

Then you will have a menu.
///////////////////
1. show product
2. My Shopping Cart
3. Check out
4. Exit

Selection option:
///////////////////

The program is self deriving. So you can easily go through the code.

For more information contack me.
www.mihmilon.com
