// AirBox base
// All numbers are supposed to be millimeters

height = 55;
outer_size = 51;
inner_size = 43;
disp_size = 47;
wall_size = (outer_size - inner_size) / 2;

usb_x = 10;
usb_y = 7.5;

disp_inner_size = 40;

difference() {
    cube(size=[outer_size, outer_size, height], center=false);
    
    translate([wall_size, wall_size, wall_size]) {
        cube(size=[inner_size, inner_size, height*1.1], center=false);
    }
    
    translate([wall_size / 2, wall_size / 2, height - wall_size - 1]) {
        cube(disp_size, center=false);
    }
    
    translate([(outer_size - usb_x) / 2, wall_size, -15]) {
        cube(size=[usb_x, usb_y, 30], center=false);
    }
    
    for (x = [8:9:outer_size],
         y = [8:9:outer_size])
    translate([x-0.5, y-0.5, -15]) {
        cylinder(h=30, d=4, center=false, $fn=100);
    }
}

echo(version=version());

// Written by Alexey Kondratov <kondratov.aleksey@gmail.com>
