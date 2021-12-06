public class Animal {
	public Animal()
	{
		
	}

	public static void main(String[] args) {
		System.out.println("Hi from Animal");
	}

}


abstract class Mammal extends Animal
{
	static int numMammal;
	Mammal()
	{
		numMammal=0;
	}
	int getNumMammal();
	
}

abstract class Reptile extends Animal
{
	static int numReptile;
	Reptile()
	{
		numReptile=0;
	}
	int getNumReptile();
	
}

final class Cat extends Mammal
{
	String name;
	float weight;
	String nameSlave;
	
	Cat(String name) // Constructor
	{
		this.name=name;
	}
	
	int getNumMammal()
	{
		return super.numMammal;
	}
	
	void getField()
	{
		System.out.println("Cat name is: "+ name);
		System.out.println("Cat weight is: " + weight);
		System.out.println("Cat name of Slave is :" + nameSlave);
		System.out.println("numMammal: "+ getNumMammal());
		
	}
	void setName(String name)
	{
		this.name=name;
	}
	void setWeight(float weight)
	{
		this.weight=weight;
	}
	void setnameSlave(String nameSlave)
	{
		this.nameSlave=nameMaster;
	}
	
	
	void breed()
	{
		System.out.println("Cat: breeding...");
		super.numMammal+=3;
	}
	void meow()
	{
		System.out.println("("+ name +") : meow");
	}
	void sleep()
	{
		System.out.println("("+ name +") : Zzz");
	}
	
}

final class Dog extends Mammal
{
	String name;
	float weight;
	String nameMaster;
	
	Dog(String name) // Constructor
	{
		this.name=name;
	}
	int getNumMammal()
	{
		return super.numMammal;
	}
	
	void getField()
	{
		System.out.println("Dog name is: "+ name);
		System.out.println("Dog weight is: " + weight);
		System.out.println("Dog name of Master is :" + nameMaster);
		System.out.println("numMammal: "+ getNumMammal());
	}
	void setName(String name)
	{
		this.name=name;
	}
	void setWeight(float weight)
	{
		this.weight=weight;
	}
	void setnameMaster(String nameMaster)
	{
		this.nameMaster=nameMaster;
	}
	void Breed()
	{
		System.out.println("Dog : Breeding...");
		super.numMammal+=5;
	}
	void bark()
	{
		System.out.println("("+ name + ") : bowwow");
	}
}

final class Crocodile extends Reptile
{
	String name;
	float weight;
	
	Crocodile(String name) // Constructor
	{
		this.name=name;
	}
	
	int getNumReptile()
	{
		return super.numReptile;
	}
	
	void setName(String name)
	{
		this.name=name;
	}
	void setWeight(float weight)
	{
		this.weight=weight;
	}
	
	void getField()
	{
		System.out.println("Crocodile name is: "+ name);
		System.out.println("Crocodile weight is: " + weight);
		System.out.println("numReptile: "+ getNumReptile());
	}
	void spawn()
	{
		System.out.println("Crocodile: Laying eggs...");
		super.numReptile+=20;
	}
}
