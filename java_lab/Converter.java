import java.util.Scanner;

public class Converter 
{
		public static void main(String args[])
		{
				Scanner scn = new Scanner(System.in);
				int input = scn.nextInt();

				int binary = input;
				int octal = input;
				int hexadecimal = input;

				System.out.print("b ");
				int[] arr_binary =new int[20];
				int i=0;
				while(binary>2)
				{
						int temp = binary%2;
						arr_binary[i] = temp;
						binary/=2;
						i++;
				}
				i--;
				for(int j=i;j>0;j--)
				{
						System.out.print(arr_binary[j]);
				}
				System.out.println(binary);
		}
}
