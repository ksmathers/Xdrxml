#include "test.h"
struct Item item = { "Longsword" };
struct Player p = {
    "Bridget",
    { 0, 0 },
    HEALTH_DEAD,
    &item, 
    { 0, 0, NULL },
    1,
    -8, 
    32
};

int main( int argc, char **argv) {
    XDR xdrs;
    xdr_xml_create( &xdrs, "foo", XDR_ENCODE);
    xdr_Player( &xdrs, "p", &p);
    xdrxml_destroy( &xdrs);
}
