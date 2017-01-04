#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>

static char *host = "localhost";
static char *user = "root";
static char *pass = "123";
static char *dbname = "test_db";
unsigned int port = 3306;
static char *unix_socket = NULL;
unsigned int flag = 0;

MYSQL *conn;
MYSQL_RES *res;
MYSQL_RES *res2;
MYSQL_ROW row;
MYSQL_ROW row2;
int customer_session_id;

void mysql_connection(){
	if(!(mysql_real_connect(conn, host, user, pass, dbname, port, unix_socket, flag))){
		fprintf(stderr, "ERROR: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
		exit(1);
	}
}
void checkout(){
	char c, query1[1024], query2[1024];
	sprintf(query1,"select productid, name, sum(quantity), sum(price) from cart, product where cart.productid=product.id and cart.customerid=%d group by productid", customer_session_id);
	mysql_query(conn,query1);
	res = mysql_store_result(conn);
	printf("\t\t**Your shopping cart**\n\nProductID	Name		Quantity	price\n");
	while(row = mysql_fetch_row(res)){
		printf("%d\t\t%s\t\t%d\t\t%d\n", atoi(row[0]), row[1], atoi(row[2]), atoi(row[3]));
	}
	printf("\n\n");
	printf("Proceed to check out and payment?(Y/N): ");
	scanf(" %c", &c);
	if (c=='Y'|| c=='y'){
                sprintf(query2,"delete from cart where customerid=%d",customer_session_id);
		mysql_query(conn,query2);
		system("clear");
		printf("\t**Thank you for shopping..\n\n");
	}
        else{
                system("clear");
	}

	mysql_free_result(res);
}
void show_cart(){
	char query1[1024];
	sprintf(query1,"SELECT productid,name,quantity FROM cart,product WHERE customerid=%d and cart.productid=product.id",customer_session_id);
	mysql_query(conn, query1);
	res = mysql_store_result(conn);
	printf("ProductID	Name	Quantity\n");
	while(row = mysql_fetch_row(res)){
		printf("%d\t\t%s\t\t%d\n", atoi(row[0]),row[1],atoi(row[2])); 	
	}
	mysql_free_result(res);
}
int customer(int id){
	mysql_query(conn, "SELECT * FROM customer");
	res = mysql_store_result(conn);
	while(row = mysql_fetch_row(res)){
		if(id == atoi(row[0])){
			return 1;
			break;		
		}
	}
	return 0;
	mysql_free_result(res);
}
void show_product(){
	char itemName[10], *temp[30], q[1024], q2[1024], query1[1024];
	int counter=0, i, quantity, itemid[30], temp2, tempid;;
	printf("****************************\n");
	mysql_query(conn, "SELECT product.id,name,price,quantity FROM product,stock where product.id=stock.productid");
	res = mysql_store_result(conn);
	printf("ID	Name	Price	quantity\n");
	while(row = mysql_fetch_row(res)){
		printf("%d\t%s\t$%d\t%d\n", atoi(row[0]), row[1], atoi(row[2]), atoi(row[3]));
		temp[counter] = malloc(strlen(row[1]))+1;
		strcpy(temp[counter],row[1]);	
		itemid[counter]=atoi(row[0]);	
		counter++;		
	}
	printf("Type name of the product to add: ");
	scanf("%s", itemName);
	printf("quantity: ");
	scanf("%d", &quantity);	
	sprintf(query1,"select quantity,stock.productid from stock,product where product.id=stock.productid and product.name= '%s'",itemName);
	mysql_query(conn,query1);
	res2 = mysql_store_result(conn);
	while(row2 = mysql_fetch_row(res2)){
		printf("%d\n", atoi(row2[0]));
		temp2=atoi(row2[0]);
		tempid=	atoi(row2[1]);	
	}
	if(quantity<=temp2){
		for(i=0;i<counter;i++){
			if(strcmp(itemName,temp[i])==0){
				sprintf(q, "INSERT INTO cart (customerid,productid,quantity) VALUES (%d,%d,%d)", customer_session_id,itemid[i],quantity);
				sprintf(q2, "update stock set quantity=%d where productid=%d", temp2-quantity, tempid);
				if((mysql_query(conn,q)==0) && (mysql_query(conn,q2)==0)){
					system("clear");
					printf("*****%s is added to cart*****\n\n\n", itemName);
					break;
				}
			break;
			}
			else{
				printf("Product not matched\n");
			}
		}
	}
	else{
		system("clear");		
		printf("\n***Stock is low. Select less quantity\n\n");
	}
	mysql_free_result(res);
}
void menu(){
	system("clear");
    	int input=-1;
	while(input !=4){	
	    printf( "1. show product\n" );
	    printf( "2. My Shopping Cart\n" );
	    printf( "3. Check out\n" );
	    printf( "4. Exit\n" );
	    printf( "\nSelection option: " );
	    scanf( "%d", &input );
	    switch ( input ) {
		system("clear");
		case 1:            /* Note the colon, not a semicolon */
		    show_product();
		    break;
		case 2:          
		    show_cart();
		    break;
		case 3:         
		    checkout();
		    break;
		case 4:        
		    printf( "Thanks for shopping!\n" );
		    break;
		default:            
		    printf( "Bad input, quitting!\n" );
		    break;
	    }
   	getchar();
	}
}
int main(){
	int id;
	int choice = -1;
	conn = mysql_init(NULL);
	mysql_connection();
	printf("Enter your ID: ");
	scanf("%d", &id);
	if(customer(id) == 1){
		printf("authorization complete\n");
		customer_session_id = id;
		while(choice !=0){		
			menu();
			system("clear");
			printf("press 0 to quit shopping\n");
			scanf("%d", &choice);
		}
	}
	else
		printf("wrong id...\n");
	mysql_close(conn);
	return 0;
}
