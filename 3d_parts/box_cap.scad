// AirBox cap
// All numbers are supposed to be millimeters.

outer_size = 51;
inner_size = 43;
disp_size = 47;
wall_size = (outer_size - inner_size) / 2;

usb_x = 10;
usb_y = 7.5;

disp_inner_x = 35;
disp_inner_y = 40;

difference() {
    union() {
        cube([outer_size, outer_size, wall_size], center=false);
        translate([wall_size / 2, wall_size / 2, wall_size]) {
            cube([disp_size, disp_size, wall_size], center=false);
        }
    }
    translate([wall_size + 5, (outer_size - disp_inner_y) / 2, -20]) {
        cube(size=[disp_inner_x, disp_inner_y, 40], center=false);
    }
    translate([wall_size + 1, (outer_size - disp_inner_y) / 2, wall_size + 1]) {
        cube(size=[7, disp_inner_y, 5], center=false);
    }
}

echo(version=version());

// Written by Alexey Kondratov <kondratov.aleksey@gmail.com>
