// AirBox
// All numbers are supposed to be millimeters.

height = 55;
outer_size = 51;
inner_size = 43;
disp_size = 47;
wall_size = (outer_size - inner_size) / 2;

usb_x = 10;
usb_y = 7.5;

disp_inner_x = 35;
disp_inner_y = 40;

holder_wall = 2;
holder_height = 43.5;
holder_depth = 8.5;
holder_gap = 16;

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

translate([- (outer_size + 2), - (outer_size + 2), 0]) {

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

}

translate([- (outer_size + 2), 5, 0]) {

difference() {
    cube([inner_size, holder_height + holder_wall, holder_depth + holder_wall], center=false);
    translate([holder_wall, holder_wall, holder_wall]) {
        cube([inner_size - holder_wall*2, holder_height + holder_wall, holder_depth + holder_wall], center=false);
    }
    translate([(inner_size - holder_gap)/2, holder_wall, - holder_wall]) {
        cube(size=[holder_gap, holder_height + holder_wall, holder_wall*4], center=false);
    }
}

}

echo(version=version());

// Written by Alexey Kondratov <kondratov.aleksey@gmail.com>
