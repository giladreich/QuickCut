// -----------------------------

//   js index
/* =================== */
/*  
    

    

*/
// -----------------------------


(function($) {
    "use strict";



    /*---------------------
    preloader
    --------------------- */

    $(window).on('load', function() {
        $('#preloader').delay(1000).fadeOut('slow', function() { $(this).remove(); });
    });

    /*------------------------------
         counter
    ------------------------------ */
    // $('.counter-up').counterUp();

    /*---------------------
    owl carousel
    --------------------- */
    function screen_slider() {
        var owl = $(".screen-slider");
        owl.owlCarousel({
            loop: true,
            margin: 30,
            navigation: false,
            items: 4,
            smartSpeed: 1000,
            dots: true,
            autoplay: true,
            autoplayTimeout: 2000,
            dotsEach: true,
            responsive: {
                0: {
                    items: 1
                },
                480: {
                    items: 2
                },
                760: {
                    items: 4
                },
                1080: {
                    items: 4
                },
                1920: {
                    items: 4
                }
            }
        });
    }
    screen_slider();

    /*---------------------
    owl carousel
    --------------------- */
    function slider_screen_two() {
        var owl = $(".slider-screen-two");
        owl.owlCarousel({
            loop: true,
            margin: 0,
            navigation: false,
            items: 1,
            smartSpeed: 1000,
            dots: false,
            autoplay: true,
            autoplayTimeout: 2000,
            dotsEach: true,
        });
    }
    slider_screen_two();

    /*---------------------
    owl carousel
    --------------------- */
    function testimonial_list() {
        var owl = $(".testimonial-list");
        owl.owlCarousel({
            loop: true,
            margin: 0,
            navigation: false,
            items: 2,
            smartSpeed: 1000,
            dots: true,
            autoplay: false,
            autoplayTimeout: 2000,
            dotsEach: true,
            responsive: {
                0: {
                    items: 1
                },
                480: {
                    items: 1
                },
                760: {
                    items: 2
                },
                1080: {
                    items: 2
                },
                1920: {
                    items: 2
                }
            }
        });
    }
    testimonial_list();

    /*---------------------
    Sticky Header
    --------------------- */
    $(window).on('scroll', function() {
        var Wscroll = $(window).scrollTop();
        var header_area = $('.header-area');
        var navbar_fixed_top = 'header_fix';

        if (Wscroll > 100) {
            header_area.addClass(navbar_fixed_top);
        } else {
            header_area.removeClass(navbar_fixed_top);
        }

    })

    //=========================
    //  Active current menu while scrolling
    //=========================

    //ACTIVE CURRENT MENU WHILE SCROLLING

    $(window).on("scroll", function() {

        activeMenuItem($(".nav-menu"));

    });

    // function for active menuitem
    function activeMenuItem($links) {
        var top = $(window).scrollTop(),
            windowHeight = $(window).height(),
            documentHeight = $(document).height(),
            cur_pos = top + 2,
            sections = $("section"),
            nav = $links,
            nav_height = nav.outerHeight(),
            home = nav.find(" > ul > li:first");


        sections.each(function() {
            var top = $(this).offset().top - nav_height - 40,
                bottom = top + $(this).outerHeight();

            if (cur_pos >= top && cur_pos <= bottom) {
                nav.find("> ul > li > a").parent().removeClass("active");
                nav.find("a[href='#" + $(this).attr('id') + "']").parent().addClass("active");
            } else if (cur_pos === 2) {
                nav.find("> ul > li > a").parent().removeClass("active");
                home.addClass("active");
            } else if ($(window).scrollTop() + windowHeight > documentHeight - 400) {
                nav.find("> ul > li > a").parent().removeClass("active");
            }
        });
    }

    //=========================
    // 8. Smoth Scroll
    //=========================

    function smoothScrolling($links, $topGap) {
        var links = $links;
        var topGap = $topGap;

        links.on("click", function() {
            if (location.pathname.replace(/^\//, '') === this.pathname.replace(/^\//, '') && location.hostname === this.hostname) {
                var target = $(this.hash);
                target = target.length ? target : $("[name=" + this.hash.slice(1) + "]");
                if (target.length) {
                    $("html, body").animate({
                        scrollTop: target.offset().top - topGap
                    }, 1000, "easeInOutExpo");
                    return false;
                }
            }
            return false;
        });
    }

    $(window).on("load", function() {
        smoothScrolling($(".main-menu > nav > ul > li > a[href^='#']"), 70);
    });

    //=========================
    // 8. Slick Nav Activation
    //=========================
    $('.nav-menu > ul').slicknav({
        'prependTo': '.mobile_menu'
    });

    //=========================
    // 8. YTP activation
    //=========================
    $(".background-video").YTPlayer();


    //=========================
    // 8. Rain Drops Canvas
    //=========================
    $('.raindrops-canvas').raindrops({
        color:'#fff',
        canvasHeight:200
    });


}(jQuery));