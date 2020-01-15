/* ************************************************************************
*  file:  mud2html.c                              Part of Dragon's Fang   *
*  Usage: Processing mud colors in files and make it htmlized             *
************************************************************************* */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
* Array of map colors between \c color codes and the numbers that the users type in to get them
* in the mud.
*/
char *colormapWithC[] =
{
  "white",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "#666666",
  "#666666",
  "#666666",
};

/*
* Array of map colors between & color codes and the numbers that the users type in to get them
* in the mud.
*/
char *colormapWithAnd[] =
{
  "white",
  "black",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "black",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "black",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

};
/*
 * Since the & color codes use characters instead of numbers, a second stage of mapping
 * has to be done for those, hence the string below.
 */
char *andmap = "nkrgybmcwKRGYBMCW01234567";

void htmlize_string(char *line, char *result)
{
  char lineout[1000], substring[1000], 
       *fg = colormapWithC[0], *bg = "black", *ptr;
  int i, j, index, bold = 0;
  
  if (!line || !result || !*line)
  {
    return;
  } 
    for (i = 0; i < strlen(line); i ++)
    {
      if (line[i] == '\\' && (i+3 < strlen(line)) && 
        line[i+1] == 'c' && isdigit(line[i+2]) && isdigit(line[i+3])) // \cxx color code
      {
        index = 10*(line[i+2]-'0') + line[i+3]-'0';
        if (index < 0)
        {
          index = 0; // Wrong color number for \c code given, ignoring this color code.
        }
        else if (index == 0)
        {
          fg = colormapWithC[0];
          bg = "black";
          bold = 0;
        }
        else if ((index < 8) || (index == 22))
        {
          fg = colormapWithC[index];
          bold = 0;
        }
        else if ((index < 15) || (index == 23))
        {
          fg = colormapWithC[index];
          bold = 1;
        }
        else
        {
          index = 0; // Wrong color number for \c code given, ignoring this color code.
        }
        i += 4;
        if (index)
        {
          if (bold)
            sprintf(lineout, "</font><font style=\"font-weight: bold; color: %s; background: %s\">", fg, bg);
          else
            sprintf(lineout, "</font><font style=\"color: %s; background: %s\">", fg, bg);
        }
        else
          memset(lineout, 0, 1000);
        // Now it's time to print out the text that is to be surroundsed by the <font> tag
        for (j = i; line[j] && line[j] != '\\' && line[j] != '&'; j ++);
        memset(substring, 0, 1000);
        strncpy(substring, (line+i), j-i);
        if (index && line[j]) // Don't type outend tag if end of string or normal text
          sprintf(lineout, "%s%s</font>", lineout, substring);
        else
          sprintf(lineout, "%s%s", lineout, substring);
        sprintf(result, "%s%s", result, lineout);
        i = j-1; // -1 since j is pointing at what might be a color code beginning
      }

      else if (line[i] == '&' && (ptr = strchr(andmap,line[i+1]))) // &x color code
      {
        index = ptr-andmap; // Which index in the andmap was pointed out?

        if (index == 0)
        {
          fg = colormapWithAnd[0];
          bg = "black";
          bold = 0;
        }
        else if (index < 9)
        {
          fg = colormapWithAnd[index];
          bold = 0;
        }
        else if (index < 17)
        {
          fg = colormapWithAnd[index];
          bold = 1;
        }
        else
        {
           bg = colormapWithAnd[index];
        }
        if (index)
        {
          if (bold)
            sprintf(lineout, "</font><font style=\"font-weight:bold; color: %s; background: %s\">", fg, bg);
          else
            sprintf(lineout, "</font><font style=\"color: %s; background: %s\">", fg, bg);
        }
        else
          memset(lineout, 0, 1000);
        i+=2;
        // Now it's time to print out the text that is to be surrounded by the <font> tag
        for (j = i; line[j] && line[j] != '\\' && line[j] != '&'; j ++);
        memset(substring, 0, 1000);
        strncpy(substring, (line+i), j-i);
        if (index && line[j])
          sprintf(lineout, "%s%s</font>", lineout, substring);
        else
          sprintf(lineout, "%s%s", lineout, substring);
        sprintf(result, "%s%s", result, lineout);
        i = j-1;  // -1 since j is pointing at what might be a color code beginning
      }
      // other special characters that need converting
      else if (line[i] == '&' && line[i+1] == '&')
      {
        sprintf(result, "%s&amp;", result);
        i++;
      }
      else if (line[i] == '&' && line[i+1] == '\\')
      {
        sprintf(result, "%s\\", result);
        i++;
      }
      else if (line[i] == '>')
        sprintf(result, "%s&gt;", result);
      else if (line[i] == '<')
        sprintf(result, "%s&lt;", result);
      else if (line[i] == '"')
        sprintf(result, "%s&quot;", result);
      else
        sprintf(result, "%s%c", result, line[i]); // no color code, print out character to string
    }
}


/*
 * Takes a mud help file or something like that, with color codes in the text, and
 * converts it so that the colors are html-encoded and writes it down to another file.
 * For it to look as true to the layout in the game as possible, the outfile
 * must have <pre> </pre> surrounding the output that is written to file with this function.
 * Surrounding tags, such as <html>, <body> and such must be written by the function
 * calling this one.
 * It does not close either of the files; the caller controls that. 
 *
 * Parameters: File to read from and file to write HTML output to.
 * Returns: 1 if everything went ok, 0 if something went wrong.
 */
int htmlize(FILE *infile, FILE *outfile)
{
  char line[10000], *result; //, colorAtEndOfLine[100];
  if (!infile || !outfile)
  {
    return 0;
  }
  result = malloc(10000);
  while (fgets(line, 10000, infile))
  {
    memset(result, 0, 10000);
    htmlize_string(line, result);
    fprintf(outfile, result);
  }
  free(result);
  return 1;
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;

  if (argc != 3)
    fprintf(stderr, "Usage: %s mudfile htmlfile\n", argv[0]);
  else
  {
    if (!strcmp(argv[2], argv[1]))
    {
      printf("Destination file and source file can't be the same.\r\n");
      return 1;
    }

    if (!(infile = fopen(argv[1],"r")))
    {
      printf("Can't open file %s for reading.\r\n", argv[1]);
      return 1;
    }
    if (!(outfile = fopen(argv[2],"w")))
    {
      printf("Can't open file %s for writing.\r\n", argv[2]);  
      return 1;
    }
    fprintf(outfile, "<html>\r\n<head>\r\n"); 
    fprintf(outfile, "<title>%s</title></head>\r\n<body text=green  bgcolor=black>\r\n", argv[1]);
    fprintf(outfile, "<pre>\r\n");
    htmlize(infile, outfile);
    fprintf(outfile, "</pre>\r\n");
    fprintf(outfile, "\r\n<body>\r\n</html>");
    fclose(infile);
    fclose(outfile);
  }
  return 0;
}
