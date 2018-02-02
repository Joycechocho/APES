#include <stdlib.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "ddl.h"

void test_delete_from_beginning(void **state)
{
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_beginning(head,1);
  head=delete_from_beginning(head);
  assert_int_equal(0, peek_value(head,1));
}

void test_delete_from_end(void **state)
{ 
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_beginning(head,1);
  head=delete_from_end(head);
  assert_int_equal(0, peek_value(head,1));
}

void test_delete_from_position(void **state)
{
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_beginning(head,1);
  head=delete_from_position(head,1);
  assert_int_equal(0, peek_value(head,1));
}

void test_destroy(void **state)
{
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_beginning(head,1);
  head=destroy(head);
  assert_null(head);
}

int main(int argc, char **argv)
{
  const struct CMUnitTest tests[]=
  {
    cmocka_unit_test(test_delete_from_beginning),
    cmocka_unit_test(test_delete_from_end),
    cmocka_unit_test(test_delete_from_position),
    cmocka_unit_test(test_destroy),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
