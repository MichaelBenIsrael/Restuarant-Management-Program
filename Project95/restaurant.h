#ifndef _restaurant
#define _restaurant

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>


#define Prod_name_len 50 //the maximal length of a product name

typedef struct kitchen_Manager//Products manager.
{
	struct	kitchen* head;
	struct	kitchen* tail;
} kitchen_Manager;

typedef struct kitchen {
	char* pro_name;
	float price;
	int Quantity;
	int ordered;
	struct kitchen *next;
} kitchen;

typedef struct Table_Manager //Tabels manager.
{
	float Total_Price;
	struct	Table* head;
} Table_Manager;

typedef struct Table //Each table data.
{
	int table_num;
	char* Pro_Name;
	int	 Quantity;
	int Price;
	struct Table* next;
	struct Table* prev;
} Table;





int CreateProducts(FILE*, FILE*, kitchen_Manager*, FILE*);
int check_dish_name(kitchen_Manager*,char*);
void AddItems(FILE*, char*, int, kitchen_Manager*);
void OrderItem(FILE*, FILE*, int, char*, int, int, Table_Manager*, kitchen_Manager*);
void RemoveItem(FILE*, int, char*, int, int, Table_Manager*, kitchen_Manager*);
void RemoveTable(FILE*, int, int, Table_Manager*, kitchen_Manager*);
void Error_Msg(char* str);
void DeleteTable(Table_Manager*);
void DeleteList(kitchen_Manager*);
#endif 
