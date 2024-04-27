
class Main inherits IO {
    i : Int <- 0;
    main(): SELF_TYPE {
        {
            if i = 0 then
                {
                    out_string("\n");
                    i <- i + 5;
                    out_int(i);
                }
            else  
                {
                    out_string("\n");
                    i <- i / 2;
                    out_int(i);
                }
            fi
        }
    };
};