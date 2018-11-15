#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_TOKENS 4
#define MAX_DELIM 256

struct token {
  long val;
  char delim[MAX_DELIM];
};

long Tokenize(const char *str, struct token *const tokens);
void ConcatTokens(const size_t mem, const struct token *const tokens, const long tokencnt);
void Help(const char *const me);

int main(int argc, char *argv[]) {
  if (argc > 3) Help(argv[0]);

  if (argc == 1) {
    puts("0.0.1");
    return 0;
  }


  struct token tokens[MAX_TOKENS] = {0};
  long tokencnt = Tokenize(argv[1], tokens);
  long nth_token = 0;

  if (argc == 3) {
    errno = 0;
    char *endptr = NULL;
    nth_token = strtol(argv[2], &endptr, 10);
    if (errno || *endptr) Help(argv[0]);
  }
  else
    nth_token = tokencnt;

  if (tokencnt == -1 || tokencnt < 3 || nth_token < 1 || nth_token > MAX_TOKENS) Help(argv[0]);

  tokens[nth_token - 1].val++;
  for (long i = nth_token; i < tokencnt; ++i) tokens[i].val = 0;

  if (tokencnt == 3 && nth_token == 4) {
    strcpy(tokens[2].delim, "-");
    tokens[3].val = 1;
    tokencnt = 4;
  }
  else if (nth_token < 4) {
    tokens[2].delim[0] = 0;
    tokencnt = 3;
  }
  ConcatTokens(strlen(argv[1]) + 10, tokens, tokencnt); 
  return 0;
}
 
long Tokenize(const char *str, struct token *const tokens) {
  long n = -1;
  int isdelim = 1;
  size_t idx = 0;
 
  do {
    int digit = isdigit(*str);
 
    if ((!digit && n == -1) || n >= MAX_TOKENS)
      return -1;
    else if (digit) {
      if (isdelim) {
        isdelim = 0;
        idx = 0;
        ++n;
      }
      if (idx >= MAX_DELIM)
        return -1;
      tokens[n].delim[idx++] = *str;
    } else {
      if (!isdelim) {
        tokens[n].delim[idx] = 0;
        errno = 0;
        tokens[n].val = strtol(tokens[n].delim, NULL, 10);
        if (errno) return -1;
        tokens[n].delim[0] = 0;
        isdelim = 1;
        idx = 0;
      }
      if (idx >= MAX_DELIM) return -1;
      tokens[n].delim[idx++] = *str;
    }
  } while (*str++);
  return ++n;
}

void ConcatTokens(const size_t mem, const struct token *const tokens, const long tokencnt) {
  char *out = calloc(mem , 1),
       *it1 = out,
       buf[MAX_DELIM] = {0};
  const char *it2 = NULL;

  if (out) {
    for (long i = 0; i < tokencnt; ++i) {
      sprintf(buf, "%ld", tokens[i].val);
      it2 = buf;
      while ((*it1++ = *it2++));
      it1--;
      it2 = tokens[i].delim;
      while ((*it1++ = *it2++));
      it1--;
    }
    puts(out);
    free(out);
  }
  else
    exit(1);
}

void Help(const char *const me) {
  fprintf(stderr,
          "%s [str [token]]\n\n"
          "  str    String containing the version (optional)\n"
          "  token  Number of the token that shall be increased (optional)\n\n",
          me);

  exit(1);
}
