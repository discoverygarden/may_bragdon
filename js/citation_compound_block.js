var title = "none";

(function ($) {



    Drupal.behaviors.may_bragdon_citation_updater = {
        attach: function (context, settings) {
            // Monkey patch Drupal.settings.islandora_paged_tei_seadragon_update_page
            // to update compound block to ensure we always get the current one.

            var old_page_update = Drupal.settings.islandora_paged_tei_seadragon_update_page;

            Drupal.settings.islandora_paged_tei_seadragon_update_page = function (pid, page_number) {
                // Drop out here if we are the most current request.
                if (pid === Drupal.settings.islandora_paged_tei_seadragon.current_page) {
                    return;
                }

                 //run the original function
                old_page_update(pid, page_number);

                // call out to get citation information
                $.ajax({
                    url: Drupal.settings.basePath + "may_bragdon/mods/" + pid,
                    cache: false,
                    success: function(response) {
                        console.log(response);
                        if(response.mods !== null && response.mods.Page !== null){
                            console.log("we have 10 page");
                            $('#citation_page_number').html(response.mods.Page[0]);
                            title = response.mods.Title[1];
                            console.log(title);
                        } else {
                            //console.log("No page");
                            $('#citation_page_number').html("Not Found");
                        }
                    },
                    error: function(response){
                        //console.log(response);
                    }
                });
            };
        }

    };

    //call the page load function after the first page load
    $(function() {
        if( $(".openseadragon-container").length){
            $(".openseadragon-container").css("position", "absolute");

            // update the border color of the naviator once it is added to the
            // page
            $('.openseadragon-container').on('DOMNodeInserted', function(e) {
                if ($(e.target).is('.displayregion')) {
                    $(".displayregion").css("border", "2px solid rgb(95, 187, 255)");
                }
                if ($(e.target).is('span')) {
                    $(".navigator").css("border", "");
                    $(".navigator").css("border-bottom", " 2px solid rgb(85, 85, 85)");
                    $(".navigator").css("border-left", " 2px solid rgb(85, 85, 85)");
                }

            });
        }





        // update the page number on initial load
        if($("#islandora_paged_tei_seadragon_pager").length) {

            var pid = $("#islandora_paged_tei_seadragon_pager").val();
            $.ajax({
                url: Drupal.settings.basePath + "may_bragdon/mods/" + pid,
                cache: false,
                success: function(response) {
                        console.log(response);
                        if(response.mods !== null && response.mods.Page !== null){
                            console.log("we have a page");
                            $('#citation_page_number').html(response.mods.Page[0]);
                        } else {
                            console.log("No page");
                            $('#citation_page_number').html("Not Found");
                        }
                },
                error: function(response){
                    //console.log(response);
                }
            });

        }
    });


})(jQuery);
