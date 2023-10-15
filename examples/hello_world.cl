class Main {
    i : IO <- new IO;
    main() :IO { i.out_string("Hello World!\n") };
};

-- class Main {
--     main() :IO { (new IO).out_string("Hello World!\n") };
-- }

-- class Main inherits IO {
--     main() :Object { self.out_string("Hello World!\n") };
-- }

-- class Main inherits IO {
--     main() :Object { out_string("Hello World!\n") };
-- }