// test_ifdef.c 
#define TEST 0
#define TEST_VAR1 (0)

#ifdef TEST
void enable()
{
}
#else
void disable()
{
}
#endif

/* main */
void main()
{
}
