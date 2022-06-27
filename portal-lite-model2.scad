use <../modely/boxes.scad>
use <../modely/esp32.scad>
use <../modely/charger-module.scad>
use <../modely/buttons.scad>
use <../modely/mfrc.scad>

DEBUG = true;
//DEBUG = false;

fatness = 1.4;
inset = .2;

shaft_inset = .4;

round_prec = 40;

// ---------------------------------------------

pcb_size = [60, 40, 1.6];
rtc_size = [27, 28, 9.56];

display_board_size = [37.9, 2.1, 12.2];
display_size = [26.1, 11.5];
display_pos = [5.9, 0.3];

esp32_size = ESP32_size();
charger_size = Charger_size();

buttons_spacing = 27;

// ---------------------------------------------

height = 38;

outter_dia = 100;
outter_height = 30;

battery_length = 66;
battery_dia = 19;
batt_holder_size = [66 + 2 * 1.5, battery_dia + 2 * 1.5, battery_dia + 2 * 1.5];

// ---------------------------------------------

module Board() {
    esp_pos = [8.2, 8, 11.2];
    rtc_pos = [- 15.1, esp_pos.y + 0.2, 0];

    color("gray") cube(pcb_size);
    color("red") translate([12, esp_pos.y, pcb_size.z]) cube([38.5, 2.4, esp_pos.z]);
    color("red") translate([12, esp_pos.y + esp32_size.y - 2.4, pcb_size.z]) cube([38.5, 2.4, esp_pos.z]);
    color("black") translate([esp_pos.x, esp_pos.y, esp_pos.z + pcb_size.z]) ESP32();
    color("black") translate([rtc_pos.x, rtc_pos.y, rtc_pos.z + pcb_size.z]) cube(rtc_size);
}

module Display() {
    union() {
        color("gray") cube(display_board_size);
        color("green") translate([display_pos.x, - 1.6, display_pos.y]) cube([display_size.x, 1.6, display_size.y]);
        color("black") translate([0, display_board_size.y, 1]) cube([2.5, 8, 9.8]);
        color("black") translate([0, - 1, 1]) cube([2.5, 1, 9.8]);
        color("gray") translate([0, - 1.1]) rotate([90]) translate([8, 5]) text("Display", size = 5);
    }
}

module BatteryHolder() {
    difference() {
        cube(batt_holder_size);
        translate([1.5, 1.5]) cube([66, battery_dia, 100]);
    }
    if (DEBUG) translate([1.5, battery_dia / 2 + 1.5, battery_dia / 2 + 1.5]) color("grey") rotate([0, 90])
        cylinder(d = battery_dia, h = battery_length, $fn = 25);
}

// ---------------------------------------------


module MainPart() {
    union() {
        difference() {
            scale_y = 1.6;
            scale([1, scale_y, 1]) cylinder(d = outter_dia, h = outter_height, $fn = 100);
            translate([0, 0, fatness]) scale([1, scale_y, 1]) cylinder(d = outter_dia - 2 * fatness, h = outter_height, $fn = 100);
        }

        translate([0, 0, fatness]) {
            translate([0, 37]) rotate([0, 0, - 90]) translate([- MFRC_board_size().x / 2, - MFRC_board_size().y / 2]) {
                if (DEBUG) translate([1 + inset, 1 + inset]) MFRC_board();

                difference() {
                    outter = [MFRC_board_size().x + 2 * inset + 2, MFRC_board_size().y + 2 * inset + 2, 5];
                    cube(outter);
                    translate([1, 1]) cube([MFRC_board_size().x + 2 * inset, MFRC_board_size().y + 2 * inset, 100]);
                }
            }

            translate([0, - 50, 0]) {
                //                if (DEBUG) Board();
            }

            translate([- batt_holder_size.x / 2, - 16.3]) {
                BatteryHolder();
            }

            translate([- pcb_size.x / 2, - 58]) {
                cube(pcb_size);
            }
        }
    }

}

// ---------------------------------------------

// main module
difference() {
    union() {
        MainPart();
    }


    //    // display hole
    //    translate([(main_box.x - display_size.x) / 2, - .01, 24.5]) cube([display_size.x, fatness + .02, display_size.y - 1]);
    //
    //    // button holes
    //    for (i = [- 1:2:1]) {
    //        translate([(main_box.x - button_size().x) / 2 + (i * buttons_spacing) + button_size().x / 2, - .01, 21 + button_size().z / 2])
    //            rotate([- 90]) cylinder(d = button_hole_dia(), h = fatness + .02, $fn = round_prec);
    //    }
    //
    //    // switch hole
    //    translate([- .01, 37.4, 9.6]) rotate([0, 90]) cylinder(d = switch_hole_dia(), h = fatness + .02, $fn = round_prec);
    //
    //    // charger hole
    //    translate([- .01, 2, 4.85]) color("red") cube([fatness + .02, 4.3, 9.7]);

}
