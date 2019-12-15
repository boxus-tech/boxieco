// AirBox Arduino Nano v3 inner holder
// All numbers are supposed to be millimeters.

inner_size = 43;
holder_wall = 2;
holder_height = 43.5;
holder_depth = 8.5;
holder_gap = 16;

difference() {
    cube([inner_size, holder_height + holder_wall, holder_depth + holder_wall], center=false);
    translate([holder_wall, holder_wall, holder_wall]) {
        cube([inner_size - holder_wall*2, holder_height + holder_wall, holder_depth + holder_wall], center=false);
    }
    translate([(inner_size - holder_gap)/2, holder_wall, - holder_wall]) {
        cube(size=[holder_gap, holder_height + holder_wall, holder_wall*4], center=false);
    }
}

echo(version=version());

// Written by Alexey Kondratov <kondratov.aleksey@gmail.com>
