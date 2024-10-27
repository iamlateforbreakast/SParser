extern int run_UT_List_01();

typedef struct TestDefinition TestDefinition;

struct TestDefinition
{
  char *  name;
  int (*run)();
};

TestDefinition tests[] =
{
  { "UT_List_01", run_UT_List_01 }
};

int main()
{
  int nbTests = sizeof(tests)/sizeof(TestDefinition);
  
  for (int i = 0; i++; i<nbTests)
  {
    int result = tests[i].run();
  }
}
