#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/***********************************************************
* Function: struct node *destroy(struct node *head);
*
* Description: Destroy all nodes in the linked list by freezing
*              the memory. 
*
* Input Parameters: a linked list data structre pointer
*
* Return: Pointer to the head of the linked list
************************************************************/
struct node *destroy(struct node *head);

/************************************************************
* Function: struct node *insert_at_beginning(struct node *head, uint32_t data)
*
* Description: Add a node to the beginning of the linked list. 
*
* Input Parameters: the head node pointer and the data to add.
*
* Return: Pointer to the head of the linked list
************************************************************/
struct node *insert_at_beginning(struct node *head, uint32_t data);

/************************************************************
* Function:struct node *insert_at_end(struct node *head, uint32_t data)
*
* Description: Add a node to the end of the linked list. 
*
* Input Parameters: the head node pointer and the data to add.
*
* Return: Pointer to the head of the linked list
************************************************************/
struct node *insert_at_end(struct node *head, uint32_t data);

/************************************************************
* Function:struct node *insert_at_position(struct node *head, uint32_t data, uint32_t position)
*
* Description: Add a node to a given position of the linked list. 
*
* Input Parameters: a base node pointer, data to add and the index
*                   of where to add the data.
*
* Return: Pointer to the head of the linked list
************************************************************/
struct node *insert_at_position(struct node *head, uint32_t data, uint32_t position);

/************************************************************
* Function:struct node *delete_from_beginning(struct node *head)
*
* Description: Delete a node from the beginning of the linked list. 
*
* Input Parameters: The head node pointer
*
* Return: Pointer to the head of the linked list
************************************************************/
struct node *delete_from_beginning(struct node *head);

/************************************************************
* Function:struct node *delete_from_end(struct node *head)
*
* Description: Delete a node from the end of the linked list. 
*
* Input Parameters: The head node pointer
*
* Return: Pointer to the head of the linked list
************************************************************/
struct node *delete_from_end(struct node *head);

/************************************************************
* Function:struct node *delete_from_position(struct node *head)
*
* Description: Delete a node from any given position of the linked list. 
*
* Input Parameters: The head node pointer
*
* Return: Pointer to the head of the linked list
************************************************************/
struct node *delete_from_position(struct node *head, uint32_t position);

/************************************************************
* Function:struct node *peek_value(struct node *head, uint32_t index)
*
* Description: Return the data from the linked list item at 
*              a given position.
*
* Input Parameters: A base node pointer and the index to where 
*                   to get data from 
*
* Return: The data of the item pointer you have
************************************************************/
struct node *peek_value(struct node *head, uint32_t index);


/************************************************************
* Function:struct node *size(struct node *head)
*
* Description: Determine the number of items in the linked list.
*
* Input Parameters: A node pointer. 
*
* Return: Size of linked list in nodes.
************************************************************/
struct node *size(struct node *head);

struct node *create_empty_list(struct node *head, uint32_t data);
void display(struct node *head);


