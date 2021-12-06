import java.util.Scanner;

public class StringMethod
{
		static String addString(String s1, int index, String s2)
		{
				String rString = new String();
				for(int i=0;i<=index;i++)
				{
						rString += s1.charAt(i);
				}
				rString += s2;
				for(int i=index+1;i<s1.length();i++)
				{
						rString += s1.charAt(i);
				}
				return rString;
		}
		static String reverse(String s)
		{
				String r_string="";
				for(int i=s.length();i>0;i--)
				{
						char tempc = s.charAt(i-1);
						String temps = "" + tempc;
						r_string=r_string.concat(temps);
				}
				return r_string;
		}
		static String removeString(String s1, String s2)
		{
				while(s1.contains(s2))
				{
						s1 = s1.replace(s2,"");
				}
				return s1;
		}
		public static void main(String args[])
		{
				Scanner scn = new Scanner(System.in);
				String input = scn.next();
				System.out.println(addString("0123456",3,"-"));
				System.out.println(reverse(input));
				System.out.println(removeString("01001000","00"));
		}
}
