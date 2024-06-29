(* this is a multiline comment test (*
testing also the nesting. *)
*)

class Main inherits IO {
   main(): Int {
	out_string("Hello, World.\n")
   };

   other(): SELF_TYPE {
	out_string("Hello, World.\n")
   };

   another(): SELF_TYPE {
	out_string("Hello, World.\n")
   };
};
-- this is short comment.