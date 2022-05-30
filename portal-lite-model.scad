use <../modely/boxes.scad>
use <../modely/esp32.scad>
use <../modely/charger-module.scad>
use <../modely/flexbatter.scad>
use <../modely/buttons.scad>
use <../modely/mfrc.scad>

//DEBUG = true;
DEBUG = false;


fatness = .8;
inset = .2;

shaft_inset = .4;

round_prec = 40;

// ---------------------------------------------

pcb_size = [60, 40, 1.6];
rtc_size = [27, 28, 9.56];

display_board_size = [37.9, 3.7, 12.2];
display_size = [26.1, 11.5];
display_pos = [5.9, 0.3];

batt_holder_size = [77.5, 22.5, 17.2];
esp32_size = ESP32_size();
mfrc_shaft_size = Shaft_size_outter();
charger_size = Charger_size();

buttons_spacing = 27;

// ---------------------------------------------

height = 38;

main_box = [97.8, mfrc_shaft_size.y, height];
main_inner_box = [main_box.x - 2 * fatness, main_box.y - 2 * fatness, main_box.z - 2 * fatness];

main_cover_size_inner = [main_box.x + inset, main_box.y + inset, main_box.z + inset];
main_cover_size_outter = [main_cover_size_inner.x + 2 * fatness, main_cover_size_inner.y + 2 * fatness, main_cover_size_inner.z + fatness];

echo("Main - Box size ", main_box.x, " x ", main_box.y, " x ", main_box.z);
echo("Main - Inner box size ", main_inner_box.x, " x ", main_inner_box.y, " x ", main_inner_box.z);

handholder_inner_box = [batt_holder_size.y - .2, batt_holder_size.x, 21];
handholder_box = [handholder_inner_box.x + 1 + 2 * fatness, handholder_inner_box.y + 1 + 2 * fatness, handholder_inner_box.z + 1 + 2 *
    fatness];

echo("HandHolder - Box size ", handholder_box.x, " x ", handholder_box.y, " x ", handholder_box.z);
echo("HandHolder - Inner box size ", handholder_inner_box.x, " x ", handholder_inner_box.y, " x ", handholder_inner_box.z);

board_pos = [35.8, 7.2, 11.6];

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
        color("green") translate([display_pos.x, - .01, display_pos.y]) cube([display_size.x, .01, display_size.y]);
        color("black") translate([0, display_board_size.y, 1]) cube([2.5, 8, 9.8]);
        color("black") translate([0, - 1, 1]) cube([2.5, 1, 9.8]);
        color("gray") rotate([90]) translate([8, 5]) text("Display", size = 5);
    }
}

module BatteryHolder() {
    translate([11.45, 11.4]) flexbatter18650();
}

// ---------------------------------------------

module MainPart() {
    union() {
        difference() {
            cube(main_box);
            translate([fatness, fatness, fatness]) {
                cube([main_inner_box.x, main_inner_box.y, main_box.z]);
            }

            // MFRC shaft hole
            translate([main_box.x - 10, - .01]) cube([10.01, main_box.y + .02, mfrc_shaft_size.z]);

            // DEBUG:
            // translate([fatness, - .01, mfrc_shaft_size.z]) cube([main_box.x - 2 * fatness, 100, 100]); // front & back
            // translate([- .01, fatness]) cube([fatness + .02, main_box.y - 2 * fatness, 100]); // left
            // translate([main_box.x - fatness - .01, fatness]) cube([fatness + .02, main_box.y - 2 * fatness, 100]); // right
        }

        translate([main_box.x - mfrc_shaft_size.x, 0]) difference() {
            MFRC_shaft();
            translate([- .01, 0]) cube([1, mfrc_shaft_size.y, 100]);
        }

        if (DEBUG) translate(board_pos) Board();
        if (DEBUG) translate([28.8, 1.87, 18.3]) rotate([0, 90, 180]) Charger();

        // charger slot
        translate([fatness + charger_size.z - 2, 0, fatness]) difference() {
            cube([3, 3, charger_size.x]);
            translate([- .01, 0]) cube([2.15, 1.94, 100]);
        }

        if (DEBUG) {
            translate([17.55, 37.4, 9.6]) rotate([0, - 90]) Switch(fatness + inset);
            translate([(main_box.x - display_board_size.x) / 2, fatness + 1 + .1, 23.2]) Display();

            // buttons
            for (i = [- 1:2:1]) {
                translate([(main_box.x - button_size().x) / 2 + (i * buttons_spacing), - button_top_height() + 4, 21]) Button();
            }
        }

        // board bed
        translate([board_pos.x - 1 - inset, board_pos.y - 1 - inset, mfrc_shaft_size.z - .01]) difference() {
            cube([pcb_size.x + 2 * (1 + inset), pcb_size.y + 2 * (1 + inset), 6]);
            translate([1, 1]) cube([pcb_size.x + 2 * inset, pcb_size.y + 2 * inset, 100]);
            // rtc hole
            translate([- .01, 9, 3.5]) cube([10, rtc_size.y + 1, 100]);
        }

        // supports for buttons & display support
        union() {
            translate([11.09, 9.104]) {
                cube([3, 3, 22]);
            }
            translate([0, 13.6, 0]) cube([fatness + 2, 2, 32]);

            translate([main_box.x - fatness - 1, 0, mfrc_shaft_size.z]) difference() {
                cube([1 + .01, 15, 21.4]);
                translate([- .01, 2.8, 8.25]) cube([100, 10.9, 100]);
            }
        }
    }
}

module HandHolder() {
    difference() {
        roundedCube(handholder_box, 2, false, false);
        translate([fatness + .5, fatness + .5, fatness + .5]) {
            cube([handholder_inner_box.x, handholder_inner_box.y, handholder_box.z]);
        }
    }

    translate([batt_holder_size.y + 1.4, 1.5, 1]) rotate([0, 0, 90]) BatteryHolder();
}

// ---------------------------------------------

// main module
difference() {
    union() {
        MainPart();
        translate([(main_box.x - handholder_box.x) / 2, - handholder_box.y + fatness + .5, - .19]) HandHolder();
    }

    // cable hole for battery
    translate([(main_box.x - handholder_box.x) / 2 + 5, - .01, 16]) cube([handholder_box.x - 2 * 5, fatness + .52, 6.5]);

    // display hole
    translate([(main_box.x - display_size.x) / 2, - .01, 23.5]) cube([display_size.x, fatness + .02, display_size.y]);

    // button holes
    for (i = [- 1:2:1]) {
        translate([(main_box.x - button_size().x) / 2 + (i * buttons_spacing) + button_size().x / 2, - .01, 21 + button_size().z / 2])
            rotate([- 90]) cylinder(d = button_hole_dia(), h = fatness + .02, $fn = round_prec);
    }

    // switch hole
    translate([- .01, 37.4, 9.6]) rotate([0, 90]) cylinder(d = switch_hole_dia(), h = fatness + .02, $fn = round_prec);

    // charger hole
    translate([- .01, 1.7, 4.9]) cube([fatness + .02, 4, 9.5]);
}

// buttons & display support
//translate([fatness + inset, 3, 19])
translate([0, 55, 0])
    difference() {
        size = [main_box.x - 2 * fatness - 2 * inset, 10.5, 13];
        union() {
            cube(size);
            translate([0, - 2]) cube([2, 3, size.z]);
        }

        // buttons
        for (i = [- 1:2:1]) translate([(size.x - button_size().x) / 2 + (i * buttons_spacing) - inset, - .01, 1.9]) {
            cube([button_size().x + .4, 4, 100]);
            neck_width = 7;
            translate([(button_size().x - neck_width) / 2 + .2, 3.9]) cube([neck_width, 1, 100]);
            translate([0, 4.7]) cube([button_size().x + .4, 4.5, 100]);
        }

        // display
        translate([(size.x - display_board_size.x) / 2 - inset, - .01, 4]) {
            cube([2.9, 100, 10]);
            cube([display_board_size.x + .4, 2.8, 10]);
        }

        // left support hole
        translate([9.95, 5.95, - .01]) cube([3.3, 3.3, 3.1]);
    }
