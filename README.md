# Xdrxml Project
An implementation of XDR (the RPC serialization layer) that uses XML as its data encoding.  


# Background

This project was originally intended to be a networked RPG named 
"Dragon's Reach".  However a change of jobs and other interests 
intruded and the project was never finished, but it does contain 
one fairly interesting piece.


# Xdrxml Library and Compiler

This is the Xdrxml library.  At the time I wrote this code XML was 
new and shiny and I was fascinated by the idea of loose coupling that 
was made possible in XML, so while other people tried to shoehorn tightly 
coupled constraints with standards like XML-Schema, I was experimenting
with ways of making RPC calls and XDR data exchanges less dependent on
exactly matching protocol versions by using XML as the data exchange 
representation.   Xdrxml doesn't implement the full 'rpcgen' syntax, but 
it does generate code for serializing and deserializing complex data
structures into and out of XML while keeping a natural feeling XML 
document structure that is mostly missing from XML-Schema.

The XDR implementation included here extends Sun Microsystems' XDR with 
a few extra functions in the call block that add names (annotations) to 
the parts of the structure being serialized or deserialized.   Thus trying
to use this XDR library with Sun's 'rpcgen' will result in pretty ugly XML.
Instead of that I have included a compiler that generates C from IDL that
uses the additional functions to get a much nicer serialized format.


 
