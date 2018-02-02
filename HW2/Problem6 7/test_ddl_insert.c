#include <stdlib.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "ddl.h"

void test_insert_at_beginning(void **state)
{
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_beginning(head,1);
  assert_int_equal(9, peek_value(head,1));
}

void test_insert_at_end(void **state)
{
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_end(head,1);
  assert_int_equal(1, peek_value(head,1));
}

void test_insert_at_position(void **state)
{
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_beginning(head,1);
  head=insert_at_position(head,5,2);
  assert_int_equal(5, peek_value(head,1));
}

void test_size(void **state)
{
  struct node *head=NULL;
  head=create_empty_list(head,9);
  head=insert_at_beginning(head,1);
  head=insert_at_position(head,5,2);
  assert_int_equal(3, size(head));
}

int main(int argc, char **argv)
{
  const struct CMUnitTest tests[]=
  {
    cmocka_unit_test(test_insert_at_beginning),
    cmocka_unit_test(test_insert_at_end),
    cmocka_unit_test(test_insert_at_position),
    cmocka_unit_test(test_size),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
