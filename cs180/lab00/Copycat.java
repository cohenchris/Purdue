/**
 * CS180 - Lab 01
 * 
 * This program will show a message box to the user to prompt them to enter some uinput. Then, it will display a message that contains the same text that the user entered.
 * 
 * @author Chris Cohen, cohen50@purdue.edu, Section 06
 * 
 * @version January 10, 2018
 * 
 **/
import javax.swing.*;
public class Copycat{
  public static void main(String args[]){
  String s = JOptionPane.showInputDialog("Trigger the copycat: ");
  JOptionPane.showMessageDialog(null, s);
}
}