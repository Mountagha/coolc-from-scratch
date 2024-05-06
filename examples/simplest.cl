class A {};
class B inherits A {};
class Main inherits IO {
    i : Object;
    j : Int <- 5; 
    io: IO <- new IO;
    main(): String {
        {
            i <- case j of 
               -- c : Object => 7; -- out_string(b); 6;)
               b : A => 1; -- out_string(b); 6;)
               a : Object => io.out_int(10);  
              	a : IO => io.out_string("Int taken\n");
	    esac;
	    "return";
        }
    };
};
