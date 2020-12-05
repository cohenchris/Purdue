int myprintf(const char *, ...);

const char * ordinal[13] = { "zero???",
    "first", "second", "third", "fourth",
    "fifth", "sixth",  "seventh", "eighth",
    "ninth", "tenth",  "eleventh", "twelfth"
};

const char * numbers[13] = {
    "Zero", "A", "Two", "Three", "Four",
    "Five", "Six", "Seven", "Eight", "Nine",
    "Ten", "Eleven", "Twelve"
};

int main() {
    int m;
    for (m = 1; m <= 12; ++m) {
        myprintf("On the %s day of Christmas, my true love sent to me:\n",
                ordinal[m]);
        int month = m;
        switch (month) {
            case 12:
                myprintf("  %s Drummers Drumming,\n", numbers[month--]);
            case 11:
                myprintf("  %s Pipers Piping,\n", numbers[month--]);
            case 10:
                myprintf("  %s Lords a-Leaping,\n", numbers[month--]);
            case  9:
                myprintf("  %s Ladies Dancing,\n", numbers[month--]);
            case  8:
                myprintf("  %s Maids a-Milking,\n", numbers[month--]);
            case  7:
                myprintf("  %s Swans a-Swimming,\n", numbers[month--]);
            case  6:
                myprintf("  %s Geese a-Laying,\n", numbers[month--]);
            case  5:
                myprintf("  %s Gold Rings,\n", numbers[month--]);
            case  4:
                myprintf("  %s Calling Birds,\n", numbers[month--]);
            case  3:
                myprintf("  %s French Hens,\n", numbers[month--]);
            case  2:
                myprintf("  %s Turtle Doves,\n  and", numbers[month--]);
            case  1:
                myprintf("  %s Partridge in a Pear Tree.\n", numbers[month--]);
            default:
                myprintf("\n");
                break;
        }
    }

    myprintf("And a Partridge in a Pear Tree!\n");
    return 0;
}
