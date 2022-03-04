#include "restaurant.h"

int main() {
	FILE* Instrucions, * Manot, * out;
	kitchen_Manager km;
	Table_Manager* arr;
	int i, command, number_of_tables, quan,table_index;
	char dish_name[Prod_name_len];

	if ((Instrucions = fopen("Instructions.txt", "rt")) == NULL)
		Error_Msg("input error");
	if ((out = fopen("output.txt", "wt")) == NULL)
	{
		fclose(Instrucions);
		Error_Msg("input error");
    }
	fscanf(Instrucions, "%d", &number_of_tables); //get the amount of tables from the file
	fscanf(Instrucions, "%d", &command); //command= the No. of the first function to run
	if (command != 1)
	{//the first func 'CreateProducts' must run first
		fprintf(out,"The kitchen was not created");
		fclose(Instrucions);
		fclose(out);
		return 0;
	}
	if ((Manot = fopen("Manot.txt", "rt")) == NULL)
	{
		fprintf(out,"Error! unable to open Manot.txt");
		fclose(Instrucions);
		fclose(out);
		exit(1);
	}
	km.head = km.tail = NULL;//reset the list
	if (CreateProducts(out,Instrucions,&km, Manot) != 1)//check if the dishes was sucsessfully scaned
	{//close all the open files 
		fprintf(out, "The kitchen was not created");
		fclose(Instrucions);
		fclose(out);
		return 0;
	}
	else
		fprintf(out, "The kitchen was created");
	//allocate memory for array of struct managers 
	arr = (Table_Manager*)malloc(sizeof(Table_Manager) * (number_of_tables)+1);
	if (arr == NULL)
	{
		DeleteList(&km);
		fclose(Instrucions);
		fclose(Manot);
		fclose(out);
		Error_Msg("Failed to allocate memory");
	}
	for (i = 0; i <= number_of_tables; i++) //Initialize the array.
	{ 	
		arr[i].head = NULL;
		arr[i].Total_Price = 0;
	}
	while ((fscanf(Instrucions, " %d", &command)) != EOF)
	{
		switch (command)//command value will determine which function would run
		{
		case 1:
		{
			fprintf(out, "\nThe kitchen can only be created once");
			break;
        }
		case 2:
		{
			fscanf(Instrucions, " %s %d", dish_name, &quan);
			AddItems(out,dish_name, quan, &km);
			break;
        }
		case 3:
		{
			fscanf(Instrucions, " %d %s %d", &table_index, dish_name, &quan);
			OrderItem(out, Instrucions, table_index, dish_name, quan, number_of_tables, arr, &km);
			break;
        }
		case 4:
		{
			fscanf(Instrucions, " %d %s %d", &table_index, dish_name, &quan);
			RemoveItem(out, table_index, dish_name, quan, number_of_tables, arr, &km);
			break;
		}
		case 5:
		{
			fscanf(Instrucions, " %d ", &table_index);
			RemoveTable(out, table_index, number_of_tables, arr, &km);
			break;
		}
		}
		

	}



}
