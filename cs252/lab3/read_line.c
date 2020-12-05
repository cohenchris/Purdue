#include "read_line.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>

#include "tty_raw_mode.h"

#define VALID_R_SEARCH "(reverse-i-search)'"
#define FAILED_R_SEARCH "(failed reverse-i-search)'"

//extern void tty_raw_mode(void);

int g_line_length = 0;
int g_cursor_position = 0;
int g_r_search_index = 0;
int g_history_index = 0;
int g_history_length = 0;
bool g_using_r_search = false;
bool g_r_search_success = true;
char g_line_buffer[MAX_BUFFER_LINE];
char g_old_buffer[MAX_BUFFER_LINE];
char g_ctrl_r_prompt[MAX_BUFFER_LINE];
char * g_current_r_command = "";
char ** g_history = NULL;

/*
 * Moves the cursor over to the left one character.
 */

void left_arrow() {
  char ch = 27;
  write(1, &ch, 1);

  ch = 91;
  write(1, &ch, 1);

  ch = 68;
  write(1, &ch, 1);
} /* left_arrow() */

/*
 * Moves the cursor over the the right one character.
 */

void right_arrow() {
  char ch = 27;
  write(1, &ch, 1);

  ch = 91;
  write(1, &ch, 1);

  ch = 67;
  write(1, &ch, 1);
} /* right_arrow() */

/*
 * Moves the cursor to the end of the line.
 */

void end() {
  while (g_cursor_position < g_line_length) {
    /* Continually loop and write 'right arrows' until cursor is back at
     * the front of the line.
     */

    right_arrow();
    g_cursor_position++;
  }
} /* end() */

/*
 * Moves the cursor to the beginning of the line.
 */

void home() {
  while (g_cursor_position > 0) {
    /* Continually loop and write 'left arrows' until cursor is back at the
     * front of the line.
     */

    left_arrow();
    g_cursor_position--;
  }
} /* home() */

/*
 * Searches the command history for the most suitable command suggestion.
 * Sets g_current_r_command to whatever is found.
 * Algorithm: search command history from the most recent command executed, and
 * if there is a matching character anywhere in the command, use that command.
 * If there is no command with matching substring, failure (use failure prompt).
 */

void search_command_history() {
  if ((!strcmp(g_line_buffer, "")) || !g_using_r_search) {
    return;
  }

  for (int i = g_r_search_index; i >= 0; i--) {
    char * search_result_position = strstr(g_history[i], g_line_buffer);
    if ((search_result_position) != NULL) {
      /* There is a match for the string that has been typed into the buffer.
       * Change g_current_r_command to this current string in the history.
       */

      g_current_r_command = strdup(g_history[i]);
      g_r_search_success = true;
      g_r_search_index = i;
      return;
    }
  }

  /* No matching command found in g_history, so switch prompt to failure
   * prompt.
   */

  g_r_search_success = false;
} /* search_command_history() */

/*
 *  Prints usage for read_line
 */

void read_line_print_usage() {
  char * usage = "\n"
    " ctrl-?       Print usage\n"
    " Left arrow   Moves the cursor to the left if possible\n"
    " Right arrow  Moves the cursor to the right if possible\n"
    " Up arrow     See last command in the history if possible\n"
    " Down arrow   See previous command in the history if possible\n"
    " Backspace    Deletes last character\n"
    " <ctrl-H>     Deletes last character\n"
    " Delete       Deletes the character at the cursor\n"
    " <ctrl-D>     Deletes the character at the cursor\n"
    " Home         Moves the cursor to the beginning of the line\n"
    " <ctrl-A>     Moves the cursor to the beginning of the line\n"
    " End          Moves the cursor to the end of the line\n"
    " <ctrl-E>     Moves the cursor to the end of the line\n"
    " <ctrl-R>     Search the command history and suggest a command\n";

  write(1, usage, strlen(usage));
} /* read_line_print_usage() */

/*
 * Clears the entire buffer (including shell prompt).
 * This is mainly for ctrl-R because you don't use a normal shell prompt.
 */

void clear_buffer() {
  int prompt_length = 0;
  if (!g_using_r_search) {
    prompt_length = 9;
  }
  else {
    if (g_r_search_success) {
      prompt_length += strlen(VALID_R_SEARCH) + 4 + strlen(g_line_buffer) +
                       strlen(g_current_r_command);
    }
    else {
      prompt_length += strlen(FAILED_R_SEARCH) + 4 + strlen(g_line_buffer) +
                       strlen(g_current_r_command);
    }
  }

  /* Go to the end of the line so you can delete every character more easily */

  end();

  /* Delete every character in the buffer (including shell prompt) */
  for (int i = 0; i < g_line_length + prompt_length; i++) {

    // Go back one character to get to the one you want to

    char * delete_character = "\b \b";
    write(1, delete_character, strlen(delete_character));

    /* Overwrite the stray character at the end that's from the original
     * string.
     */

    delete_character = " \b";
    write(1, delete_character, strlen(delete_character));

    /* Cursor is moved to end for some reason. Move it back to where it
     * should be.
     */

    for (int i = 0; i < g_line_length - g_cursor_position; i++) {
      /* Use 'left arrows' to move the cursor to the position that is
       * one space before the original position.
       */

      left_arrow();
    }
  }
} /* clear_buffer() */

/*
 * Prints ctrl-R prompt in the form of::
 * (reverse-i-search) '<buffer input>': <command guess>
 */

void write_ctrl_r_prompt() {
  if (!g_using_r_search) {
    return;
  }

  clear_buffer();

  /* Clears the current prompt so you don't have duplicates. */

  memset(g_ctrl_r_prompt, 0, strlen(g_ctrl_r_prompt));

  search_command_history();

  if (g_r_search_success) {
    strcat(g_ctrl_r_prompt, VALID_R_SEARCH);
  }
  else {
    strcat(g_ctrl_r_prompt, FAILED_R_SEARCH);
  }

  strcat(g_ctrl_r_prompt, g_line_buffer);
  strcat(g_ctrl_r_prompt, "': ");
  strcat(g_ctrl_r_prompt, g_current_r_command);

  write(1, g_ctrl_r_prompt, strlen(g_ctrl_r_prompt));
  g_line_length = strlen(g_line_buffer);
  g_cursor_position = strlen(g_line_buffer);
} /* write_ctrl_r_prompt() */

/*
 * Input a line with some basic editing.
 */

char * read_line() {
  // Set terminal in raw mode

  tty_raw_mode();

  g_line_length = 0;
  g_cursor_position = 0;

  memset(g_line_buffer, 0, MAX_BUFFER_LINE);
  memset(g_old_buffer, 0, MAX_BUFFER_LINE);

  // Read one line until enter is typed

  while (1) {
    // Read one character in raw mode.

    char ch = '\0';
    read(0, &ch, 1);

    if ((ch >= 32) && (ch != 127)) {
      // It is a printable character.

      write(1, &ch, 1);

      // If max number of character reached return.

      if (g_line_length == (MAX_BUFFER_LINE - 2)) {
        break;
      }

      for (int i = g_line_length; i > g_cursor_position; i--) {
        /* Move characters after the cursor over 1 space so they aren't
         * overwritten.
         */

        g_line_buffer[i] = g_line_buffer[i - 1];
      }

      // add char to buffer.

      g_line_buffer[g_cursor_position] = ch;
      g_line_length++;
      g_cursor_position++;

      /* Update characters after cursor */

      write(1, g_line_buffer + g_cursor_position,
            g_line_length - g_cursor_position);

      char * delete_character = " \b";
      write(1, delete_character, strlen(delete_character));

      for (int i = g_cursor_position; i < g_line_length; i++) {
        /* Move the cursor back to its original position using 'left arrows' */

        left_arrow();
      }
    }
    else if ((ch == 18) && (!g_using_r_search)) {
      // <Ctrl-R> was typed. Search command history.

      g_using_r_search = true;
      g_r_search_index = g_history_length - 1;
      memset(g_line_buffer, 0, MAX_BUFFER_LINE);
      write_ctrl_r_prompt();
    }
    else if ((ch == 18) && (g_using_r_search)) {
      // <Ctrl-R> was typed again. Get next command from command history that
      // matches, starting from index g_r_search_index.
      if (g_r_search_success) {
        g_r_search_index--;
        write_ctrl_r_prompt();
      }
    }
    else if (ch == 10) {
      // <Enter> was typed. Return line
      // Print newline

      if (g_using_r_search) {
        /* If we were just in the ctrl-R search, change the found string to the
         * suggested one.
         */

        memset(g_line_buffer, 0, MAX_BUFFER_LINE);
        strcpy(g_line_buffer, g_current_r_command);
        g_line_length = strlen(g_current_r_command);
      }
      write(1, &ch, 1);
      g_using_r_search = false;
      break;
    }
    else if (ch == 31) {
      // ctrl-?

      if (g_using_r_search) {
        continue;
      }

      read_line_print_usage();
      g_line_buffer[0] = 0;
      break;
    }
    else if ((ch == 8) || (ch == 127)) {
      // <backspace> (ctrl-H OR backspace key) was typed.
      // Remove previous character read.
      // Go back one character.

      if (g_using_r_search) {
        /* Backspace concerning ctrl-R.
         * You have to immediately update the buffer and re-print the ctrl-R
         * line instead of just the line after the shell prompt.
         */

        g_line_buffer[g_line_length - 1] = '\0';
        g_line_length--;
        g_r_search_index = g_history_length - 1;
        write_ctrl_r_prompt();
        continue;
      }

      if ((g_cursor_position == 0) || (g_line_length == 0)) {
        /* Makes sure you can't accidentally backspace the shell prompt */

        continue;
      }

      for (int i = g_cursor_position; i < g_line_length; i++) {
        /* Shift all characters after the cursor to the left to prevent a space
         * from appearing
         */

        g_line_buffer[i - 1] = g_line_buffer[i];
      }

      g_line_length--;
      g_cursor_position--;

      // Delete the character preceding the cursor

      char * delete_character = "\b \b";
      write(1, delete_character, strlen(delete_character));

      /* Update letters after cursor that were changed. */

      write(1, g_line_buffer + g_cursor_position,
            g_line_length - g_cursor_position);

      /* Overwrite the stray character at the end that's from the original
       * string.
       */

      delete_character = " \b";
      write(1, delete_character, strlen(delete_character));

      /* Cursor is moved to end for some reason. Move it back to where it
       * should be.
       */

      for (int i = 0; i < g_line_length - g_cursor_position; i++) {
        /* Use 'left arrows' to move the cursor to the position that is
         * one space before the original position.
         */

        left_arrow();
      }
      write_ctrl_r_prompt();
    }
    else if (ch == 4) {
      // <> (ctrl-D) was typed. Remove character at the cursor.
      //The characters in the right side are shifted to the left.

      if (g_using_r_search) {
        continue;
      }

      if (g_cursor_position == g_line_length) {
        /* Makes sure you can't accidentally  the shell prompt */
        continue;
      }

      for (int i = g_cursor_position + 1; i < g_line_length; i++) {
        g_line_buffer[i - 1] = g_line_buffer[i];
      }
      g_line_length--;

      /* Update characters after cursor */

      write(1, g_line_buffer + g_cursor_position,
            g_line_length - g_cursor_position);

      /* Overwrite the stray character at the end that's from the original
       * string.
       */

      char * delete_character = " \b";
      write(1, delete_character, strlen(delete_character));

      /* Cursor is moved to end for some reason. Move it back to where it
       * should be.
       */

      for (int i = 0; i < g_line_length - g_cursor_position; i++) {
        /* Use 'left arrows' to move the cursor to the position that is
         * one space before the original position.
         */

        left_arrow();
      }
    }
    else if (ch == 1) {
      // <home> (ctrl-A) was typed. Put cursor at the front of the line.

      if (g_using_r_search) {
        continue;
      }

      home();
    }
    else if (ch == 5) {
      // <end> (ctrl-E) pressed. Move cursor to the end of the line.

      if (g_using_r_search) {
        continue;
      }

      end();
    }
    else if (ch == 27) {
      // Escape sequence. Read two chars more

      char ch1 = '\0';
      char ch2 = '\0';
      read(0, &ch1, 1);
      read(0, &ch2, 1);

      if ((ch1 == 91) && (ch2 == 65)) {
        // Up arrow. Print next line in history.
        // Erase old line

        if (g_history_index == 0) {
          continue;
        }

        if (g_using_r_search) {
          continue;
        }

        if (g_history_index == g_history_length) {
          strcpy(g_old_buffer, g_line_buffer);
        }

        g_history_index--;

        // Print backspaces

        for (int i = 0; i < g_line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        // Print spaces on top

        for (int i = 0; i < g_line_length; i++) {
          ch = ' ';
          write(1, &ch, 1);
        }

        // Print backspaces

        for (int i = 0; i < g_line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        // Copy line from history

        strcpy(g_line_buffer, g_history[g_history_index]);
        g_line_length = strlen(g_line_buffer);

        // echo line

        write(1, g_line_buffer, g_line_length);
      }
      else if ((ch1 == 91) && (ch2 == 66)) {
        // Down arrow. Print previous line in history
        // Erase old line

        if (g_history_index == g_history_length) {
          continue;
        }

        if (g_using_r_search) {
          continue;
        }

        // Print backspaces

        for (int i = 0; i < g_line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        // Print spaces on top

        for (int i = 0; i < g_line_length; i++) {
          ch = ' ';
          write(1, &ch, 1);
        }

        // Print backspaces

        for (int i = 0; i < g_line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        g_history_index++;

        // Copy line from history

        if (g_history_index == g_history_length) {
          strcpy(g_line_buffer, g_old_buffer);
          g_line_length = strlen(g_line_buffer);
        }
        else {
          strcpy(g_line_buffer, g_history[g_history_index]);
          g_line_length = strlen(g_line_buffer);
        }

        // echo line

        write(1, g_line_buffer, g_line_length);
      }
      else if ((ch1 == 91) && (ch2 == 68)) {
        // Left arrow key pressed. Move cursor to the left.
        // If the cursor is at the beginning of the line, do nothing.

        if (g_using_r_search) {
          continue;
        }

        if (g_cursor_position == 0) {
          continue;
        }

        left_arrow();
        g_cursor_position--;
      }
      else if ((ch1 == 91) && (ch2 == 67)) {
        // Right arrow key pressed. Move cursor to the right.
        // If the cursor is at the end of the line, do nothing.

        if (g_using_r_search) {
          continue;
        }

        if (g_cursor_position == g_line_length) {
          continue;
        }

        right_arrow();
        g_cursor_position++;
      }
      else if ((ch1 == 91) && (ch2 == 72)) {
        // Home key pressed. Move cursor to the beginning of the line.

        if (g_using_r_search) {
          continue;
        }

        home();
      }
      else if ((ch1 == 91) && (ch2 == 70)) {
        // End key pressed. Move cursor to the end of the line.

        if (g_using_r_search) {
          continue;
        }

        end();
      }
      else if ((ch1 == 91) && (ch2 == 51)) {
        char ch3 = '\0';
        read(0, &ch3, 1);
        if (ch3 == 126) {
          // Delete key was typed. Remove character at the cursor.
          // The characters in the right side are shifted to the left.

          if (g_using_r_search) {
            continue;
          }

          if (g_cursor_position == g_line_length) {
            /* Makes sure you can't accidentally  the shell prompt */
            continue;
          }

          for (int i = g_cursor_position + 1; i < g_line_length; i++) {
            g_line_buffer[i - 1] = g_line_buffer[i];
          }
          g_line_length--;

          /* Update characters after cursor */

          write(1, g_line_buffer + g_cursor_position,
                g_line_length - g_cursor_position);

          /* Overwrite the stray character at the end that's from the original
           * string.
           */

          char * delete_character = " \b";
          write(1, delete_character, strlen(delete_character));

          /* Cursor is moved to end for some reason. Move it back to where it
           * should be.
           */

          for (int i = 0; i < g_line_length - g_cursor_position; i++) {
            /* Use 'left arrows' to move the cursor to the position that is
             * one space before the original position.
             */

            left_arrow();
          }
        }
      }
    }
    write_ctrl_r_prompt();
  }

  // Add eol and null char at the end of string

  g_line_buffer[g_line_length] = 10;
  g_line_length++;
  g_line_buffer[g_line_length] = 0;

  return g_line_buffer;
} /* read_line() */
