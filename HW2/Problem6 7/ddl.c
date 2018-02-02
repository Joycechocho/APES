#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "ddl.h"

struct node *destroy(struct node *head)
{
  struct node * current = head;
  while(current-> prev_node != NULL)
  {
    current = current-> prev_node;
  }
  free(head);
  printf("Destroyed all Nodes\n");
  return NULL;
}

struct node *insert_at_beginning(struct node *head, uint32_t data)
{
  if(head==NULL)
  {
    printf("List is empty\n");
  }
 
  struct node *temp;
  temp = (struct node *)malloc(sizeof(struct node));
  temp->info=data;
  temp->prev_node=NULL;
  temp->next_node=head;
  head->prev_node=temp;
  head=temp;
  return head;
}

struct node *insert_at_end(struct node *head, uint32_t data)
{
  if(head==NULL)
  {
    printf("List is empty\n");
  }
    
  struct node *temp,*p;
  temp=(struct node *)malloc(sizeof(struct node));
  temp->info=data;
  p=head;
  while(p->next_node!=NULL)
      p=p->next_node;
  p->next_node=temp;
  temp->next_node=NULL;
  temp->prev_node=p;
  return head;
}

struct node *insert_at_position(struct node *head, uint32_t data, uint32_t position)
{
  uint32_t i=1;
  struct node * newNode, *temp;
  temp = head;
  while(i<position-1 && temp!=NULL)
  {
     temp = temp-> next_node;
     i++;
  }
  newNode = (struct node *)malloc(sizeof(struct node));
  newNode->info = data;
  newNode-> next_node = temp-> next_node; // Connect new node with n+1th node
  newNode-> prev_node = temp;          // Connect new node with n-1th node
  
  if(temp-> next_node != NULL)
  {
    temp-> next_node-> prev_node = newNode;
  }
  temp-> next_node = newNode;
}

struct node *delete_from_beginning(struct node *head)
{
  struct node *temp;
  temp=head;
  head=head->next_node;
  head-> prev_node =NULL;
  free(temp);
  return head;
}

struct node *delete_from_end(struct node *head)
{
  struct node *p, *q;
  for (p= head; p->next_node->next_node!=NULL; p=p->next_node);
  q=p-> next_node;
  p-> next_node =NULL;
  free(q);
}
 
struct node *delete_from_position(struct node *head, uint32_t position)
{
  uint32_t count=0;
  struct node *current;
  current= head;
  if(position==1)
  {
    head-> next_node->prev_node=NULL;
    head = head-> next_node;
  }
  else
  {
    while(current!=NULL)
    {
      count++;
      if(count==position-1)
      {
        if(current-> next_node-> next_node ==NULL)
        {
          current-> next_node =current-> next_node-> next_node;
        }
        else
        {
          current-> next_node =current-> next_node-> next_node;
          current-> next_node->prev_node=current;
          current=current-> next_node;
          break;
        }
      }
      current=current-> next_node;
    }
  }
}

struct node *peek_value(struct node *head, uint32_t index)
{
  struct node * current = head;
  uint32_t count = 0; 
  while (current != NULL)
  {
    if (count == index) 
      return(current->info);
    count++;
    current = current-> next_node;
  }
  //assert(0);
}

struct node *size(struct node *head)
{
  uint32_t count = 0;  
  struct node * current = head;  
  while (current != NULL)
  {
    count++;
    current = current->next_node;
  }
  return count;
}

struct node *create_empty_list(struct node *head, uint32_t data)
{
  struct node *temp;
  temp=(struct node *)malloc(sizeof(struct node));    
  temp->info=data;
  temp->prev_node=NULL;
  temp->next_node=NULL;
  head=temp;
  return head;
}

void display(struct node *head)
{
  struct node *p;
  if(head==NULL)
  {
    printf("List is empty\n");
    return;
  }
  p=head;
  printf("List is :\n");
  while(p!=NULL)
  {
    printf("%d ",p->info);
    p=p->next_node;
  }
  printf("\n");
}

/*
int main()
{
  struct node *head=NULL;
  head=create_empty_list(head,1);
  head=insert_at_end(head,2);
  head=insert_at_end(head,3);
  head= insert_at_beginning(head, 6);
  //head= delete_from_beginning(head);
  //head= delete_from_position(head, 3);
  //head= destroy(head);
  //head=size(head);
  //printf("count of nodes is %d", size(head));
  printf("count of nodes is %d", peek_value(head, 3));
  display(head);
}
*/
