import java.util.Scanner;

public class ChartoString
{
		public static void main(String args[])
		{
				Scanner scn = new Scanner(System.in);
				String input = scn.next();
				System.out.println(input.charAt(input.length()-1));
		}
}
