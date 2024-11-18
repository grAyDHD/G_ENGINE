This module is used for basic painting and exporting headers for gameboy advance development.
It will cmatch the GBA consoles RGB15 color values
Basic tooling, no shapes to start, just a simple square brush
Ensure this provides better workflow to drawing in say, asesprite, and using grit.

    MAIN FEATURES:
        imitate modes 3-5? is this even necessary?
        select canvas dimensions
        import/edit/export functionality
        canvas sizes:
            axb, where a represents width, and b represents height (pixels)
            where a and b can be set to any of the following:
                8, 16, 32, 64, 128
            and a can be set to 240, and only b can be set to 160 
                height and width respectively. 
        tabs for multiple canvases?
        select accurate RGB15 color values (three sliders)

        import button takes compatible resolution image
            (module idea: image processor: cropper, shaders?)
    
    
