class Main {
	l: Int;
	foo(): Int {
		{
			l <- 2;
			l;
		}
	};
	main(): Int { foo() };
};
