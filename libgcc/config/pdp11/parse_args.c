static char empty_string[1];

int
parse_args (char *cmdline, char **argv, int max_args)
{
  int argc = 1;
  char *p = cmdline;

  /* RT-11 never tells a program its own invocation name (see crt0rt.s), so
     argv[0] is just this placeholder -- real arguments start at argv[1],
     matching the usual C convention.  */
  argv[0] = empty_string;

  while (argc < max_args)
    {
      while (*p == ' ' || *p == '\t')
	p++;
      if (*p == '\0')
	break;
      argv[argc++] = p;
      while (*p != '\0' && *p != ' ' && *p != '\t')
	p++;
      if (*p == '\0')
	break;
      *p++ = '\0';
    }
  argv[argc] = 0;
  return argc;
}
