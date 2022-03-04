#include "restaurant.h"

//1
int CreateProducts(FILE *out, FILE* instruc, kitchen_Manager* km, FILE* manot) {
	kitchen* temp;
	char name[Prod_name_len+1];
	int  temp_input;
	while (fscanf(manot, "%s", name) != EOF)
	{
		if (check_dish_name(km, &name) == 0)
		{//check if the dish name has already appeared
			fprintf(out, "\nError! %s Cannot be received more than once", name);
			fscanf(manot, " %d %d", &temp_input, &temp_input);//move the file pointer
			break;
		}
		if ((temp = (kitchen*)malloc(sizeof(kitchen))) == NULL)// //Allocate memory for a dish
		{
			DeleteList(km);
			fclose(manot);
			fclose(out);
			fclose(instruc);
			Error_Msg("Memory allocation eror!");
		}
		temp->pro_name = (char*)malloc(sizeof(char) * (strlen(name) + 1)); //Allocate memory for the name of product
		if (temp->pro_name == NULL)
		{
			DeleteList(km);
			fclose(manot);
			fclose(instruc);
			fclose(out);
			Error_Msg("Memory allocation eror!");
		}
		strcpy(temp->pro_name, name);
		temp->ordered = 0;   //reset the amount of times that dish has been ordered
		fscanf(manot, "%d %f", &temp->Quantity,&temp->price);
		if (temp->Quantity <= 0 || temp->price <= 0)
		{ //check if the quantity and price are positive numbers 
			fprintf(out, "\nWrong input! was'nt able to add %s to the kitchen",name);
			free(temp->pro_name);
			free(temp);
		}
		else //if the input checks out
		{
			if (km->head == NULL)
			{//if the node i added to an empty list
				km->head = temp;
				km->tail = temp;
			}
			else 
			{
				km->tail->next = temp;
				km->tail = km->tail->next;
            }
		}
		km->tail->next = NULL;
		
    }
	if (km->head == NULL)
	{
		fclose(manot);
		return 0; //0 means that not even a single dish was successfully scaned
	}
	fclose(manot);
	return 1; //1 means that that the dishes was successfully scaned 
}

//2
void AddItems(FILE *out, char* ProductName, int Quantity, kitchen_Manager* km) {
	kitchen* temp = km->head;
	if (Quantity <= 0)
	{//check if the is quantity accepeted is zero or less
		fprintf(out, "\nWrong input! the quantity of the %s is incorrect",ProductName);
		return;
	}
	while (temp != NULL)
	{//check if the list containes the accepted product
		if (strcmp(temp->pro_name, ProductName) == 0)
		{
			temp->Quantity += Quantity;
			fprintf(out, "\n%d %s were added to the kitchen",Quantity,ProductName);
            return;
		}
		temp = temp->next;
	}
	fprintf(out, "\n%s does'nt avalible in the kitchen");
}

//3
void OrderItem(FILE* f, FILE* instruc, int TableNumber, char* ProductName, int Quantity, int max_number, Table_Manager* arr, kitchen_Manager* km)
{
	Table_Manager* temp_tm;
	Table* tab, * temp_dish, *already_orderd;
	kitchen* temp = km->head;
	int i;
	if ((TableNumber > max_number)&&(TableNumber>0))
	{//check if the table index is valid
		fprintf(f, "\nUnable to place order! The table index is incorrect ");
		return;
	}
	if (Quantity <= 0)
	{//check if the is quantity accepeted is zero or less
		fprintf(f, "\nUnable to place order! The quantity is incorrect ");
		return;
	}
	if (check_dish_name(km, ProductName) != 0)
	{//check if the dish exists in the kitchen
		fprintf(f, "\nWe don’t have %s, sorry! ", ProductName);
		return;
	}
	tab = (Table*)malloc(sizeof(Table));//allocte memory for a new node
	if (tab == NULL)
	{ //case of allocation failed than print a message, free memory allocated and exit the program
		
		for (i = 0; i <= max_number; i++)
		{
			if (arr[i].head != NULL)
				DeleteTable(&arr[i]); //Frees the allocated memory of each table.
		}
		fclose(f);
		fclose(instruc);
		DeleteList(km); //Frees the allocated memory of the products.
		Error_Msg("\n*Error allocating memory!*\n");
	}
	
	//at this point we know that the dish that the table has ordered exists
	while (temp != NULL)
	{ 
		if (strcmp(temp->pro_name, ProductName) == 0)
		{
			if (temp->Quantity < Quantity)
			{//check if the kitchen has enough of that product
				free(temp);
				fprintf(f, "\nSorry! we are'nt able to provide that much %s ", *ProductName);
				return;
			}
			temp->ordered += Quantity;//count the amount of times that dish has been ordered
			already_orderd = arr[TableNumber].head;
			while (already_orderd != NULL)
			{//check if that table has already ordered that dish before
				if (strcmp(ProductName, already_orderd->Pro_Name) == 0)
				{
					already_orderd->Quantity += Quantity;//upgrade the quantity of the previously ordered dish
					arr[TableNumber].Total_Price += (temp->price * Quantity);
					temp->Quantity -= Quantity; //update the quantity of that product in the kitchen
					fprintf(f, "\n%d %s were added to table number %d", Quantity, ProductName, TableNumber);
					return;
				}
				already_orderd = already_orderd->next;
			}
		//case of all the data is valid and this is the first time that the table ordered that dish
			temp->Quantity -= Quantity; //update the quantity of that product in the kitchen
			tab->Price = temp->price; //save the price of the requsted dish
			//at this point we can update all of the 'table' struct fields
			tab->Pro_Name = (char*)malloc(sizeof(char) * strlen(ProductName)+1);
			if (tab->Pro_Name == NULL)
			{
				for (i = 0; i <= max_number; i++)
				{
					if (arr[i].head != NULL)
						DeleteTable(&arr[i]); //Frees the allocated memory of each table.
				}
				fclose(f);
				fclose(instruc);
				DeleteList(km); //Frees the allocated memory of the products.
				free(arr); //Frees the array.
				Error_Msg("\nError allocating memory!\n"); //Exit the program and closes the files.

			}
			strcpy(tab->Pro_Name, ProductName);
			tab->Quantity = Quantity;
			tab->table_num = TableNumber;
			tab->next = NULL;
			tab->prev = NULL;
			break;
		}
		temp = temp->next; //carry on with the loop until we find the product
    }
	fprintf(f, "\n%d %s were added to table number %d", Quantity, ProductName, TableNumber);
	arr[TableNumber].Total_Price += (tab->Price*Quantity);// update the total bill for that table
	if (arr[TableNumber].head == NULL)
	{//if this is the first order from that table
		arr[TableNumber].head = tab;
	}
	else 
	{//second or above order from that table
		temp_dish = arr[TableNumber].head;
		while (temp_dish->next != NULL)
		{
			temp_dish = temp_dish->next;
		}
		temp_dish->next = tab;
		tab->prev = temp_dish;
    }
    return;
}

//4
void RemoveItem(FILE* f, int TableNumber, char* ProductName, int Quantity, int max_number, Table_Manager* arr, kitchen_Manager* km)
{
	Table* temp;
	if ((TableNumber > max_number) && (TableNumber > 0))
	{//check if the table index is valid
		fprintf(f, "\nWrong input! The table index is incorrect ");
		return;
	}
	if (arr[TableNumber].head == NULL)
	{//check if that table has an ongoing order
		fprintf(f, "\nCancelation denied! The table have'nt yet ordered");
		return;
	}
	if (check_dish_name(km, ProductName) != 0)
	{//check if the dish exists in the kitchen
		fprintf(f, "\nCancelation denied! The requested dish does'nt exist ");
		return;
	}
	temp = arr[TableNumber].head;
	while (temp != NULL)
	{
		if (strcmp(temp->Pro_Name, ProductName) == 0)
		{
			if (Quantity <= 0)
			{//check if the quantity accepeted is zero or less
				fprintf(f, "\nCancelation denied! The quantity is not valid ");
				return;
			}
			if (temp->Quantity < Quantity)
			{//check if the quantity accepeted is higher than the order quantity
				fprintf(f, "\nCancelation denied! The quantity is above the limit ");
				return;
            }
			if (temp->Quantity > Quantity)
			{//case of the clients want to cancel only part of the order made for that dish 
				arr[TableNumber].Total_Price -= temp->Price * Quantity;//update the total bill for that table
				temp->Quantity -= Quantity; //update the quantity after the cancelation
				fprintf(f, "\n%d %s was returned from table No.%d",Quantity,ProductName, TableNumber);
				return;
			}
			if (temp->Quantity == Quantity)
			{//case of the the clients want to cancel the entire order of the accepted dish
				arr[TableNumber].Total_Price -= temp->Price * Quantity;//update the total bill for that table
				fprintf(f, "\n%d %s was returned from table No.%d",Quantity,ProductName, TableNumber);
				if (temp == arr[TableNumber].head)
				{//if we need to free the head node of a two-way list
					free(temp);
					arr[TableNumber].head = NULL;
					return;
				}
				else //if we need to free a node which is'nt the head
				{
					temp->prev->next = temp->next;
					if (temp->next == NULL)
					{
						free(temp);
						return;
					}
					temp->next->prev = temp->prev;
                }
            }
			
		}
		temp = temp->next;
	}
    fprintf(f, "\nCancelation denied! That dish has'nt been ordered to that table");
	return;
}


//5
void RemoveTable(FILE* out, int table_index, int max_number, Table_Manager* arr, kitchen_Manager* km)
{
	Table* temp;
	int i, best_seller=0;
	kitchen *temp_kith;
	if (table_index > max_number|| table_index<=0)
	{//check if the table index is valid
		fprintf(out, "\nUnable to clear the table! The table index is incorrect ");
		return;
	}
	if (arr[table_index].head == NULL)
	{//check if the table has ordered any dishes
		fprintf(out, "\nUnable to clear table No.%d! The table has'nt ordered any dishes", table_index);
		return;
	}
	temp = arr[table_index].head;
	while (temp != NULL)
	{//create a report of all the dishes that a certain table has ordered 
		fprintf(out, "\nTable No.%d:\n Product name: %s\n Quantity: %d", table_index, temp->Pro_Name, temp->Quantity);
		if (temp->next == NULL)//check if this is the last node 
			break;
		temp = temp->next;
		free(temp->prev);//free the nodes that contains the orders data
		
    }
	free(temp);//free the last node
	arr[table_index].head = NULL;//table has been cleared
	fprintf(out, "\nTotal price: %g", arr[table_index].Total_Price);
	for (i = 0;i <= max_number;i++)
	{//check if this is the last table in the restaurant
		if (arr[i].head != NULL)
			return;
    }
	temp_kith = km->head;
	//at this point we know that this is the last table in the restaurant
	while (temp_kith!=NULL)
	{//find the best seller dish 
		if (best_seller < temp_kith->ordered)
		{
			best_seller = temp_kith->ordered;
			break;
		}
		temp_kith = temp_kith->next;
	}
	fprintf(out, "\nThe best seller dish is: %s, that dish was ordered %d times",temp_kith->pro_name ,best_seller);
	return;


	
}

int check_dish_name(kitchen_Manager* km, char* name) {
	kitchen *temp = km->head;
	while (temp != NULL)
	{
		if (strcmp(temp->pro_name, name) == 0)
		{//check if the name is already appeared on the list
			return 0;
		}
		temp = temp->next;
	}
	return 1;
}


void Error_Msg(char* str)
{
	printf("\n%s", str);
	exit(1);
}



//Function that free the allocated memory of a specific table.
void DeleteTable(Table_Manager* tm)
{
	while (tm->head != NULL)
	{
		Table* temp = tm->head;
		tm->head = tm->head->next;
		free(temp->Pro_Name);
		free(temp);
	}
}

//Function that free the allocated memory of all the products in the kitchen.
void DeleteList(kitchen_Manager* PM)
{
	kitchen* temp;
	while (PM->head)
	{
		temp = PM->head;
		PM->head = PM->head->next;
		if (temp->pro_name)
			free(temp->pro_name);
		free(temp);
	}
}