$(document).ready(function() {

	var minWidthForBigScreen = 1920;
	var minWidthForLeft = 1150;
	var minWidthForBlog = 1150;
	var minHeightMenu = 750;
	var minHeightNetworkCenter = 500;
	var is_chrome = navigator.userAgent.toLowerCase().indexOf('chrome') > -1; //doesn't work in chrome because of the fixed network panel
	var isWebkit = 'WebkitAppearance' in document.documentElement.style; //bug in webkit when resizing page
	var isiPad = navigator.userAgent.match(/iPad/i) != null; //to adjust font size
	var isTouchDevice = !!('ontouchstart' in window);
	
	// make svg interactive
	var svgInteractive = document.getElementById("svg-interactive");
	
	if(svgInteractive && !isTouchDevice){
		$("#svg-interactive").show();
		svgInteractive.addEventListener("load",function() {
			svgPanZoom('#svg-interactive', {
				  zoomEnabled: true,
				  controlIconsEnabled: true,
				  dblClickZoomEnabled: true,
				  minZoom: 1.0,
				  maxZoom: 3.0,
				  zoomScaleSensitivity: 1.0,
				  preventMouseEventsDefault: true,
				  center: true
				});
		}, false);
 
	}
	else if(svgInteractive)
	{
		$("#svg-mobile").show();
		//var content = document.getElementById("svg-mobile").contentDocument.documentElement;
		//if(content)content.setAttribute("transform", "scale(3.0)");
	}
	
	//if resolution higher than Full HD, adjust max-width of page (only in FireFox and IE)
	
	if($( window ).width() > minWidthForBigScreen && !isWebkit){		
		$("#Center").css("max-width", "1750px");			
		$("#Content").css("max-width", "1400px");			
	}
	
	$(window).resize(function() {
		if($(window ).width() > minWidthForBigScreen && $("#Content").css("max-width") == "1150px" && !isWebkit){
			$("#Center").css("max-width", "1750px");			
			$("#Content").animate({"max-width": "1400px"});					
		}
		else if($(window ).width() <= minWidthForBigScreen && $("#Content").css("max-width") == "1400px" && !isWebkit){
			$("#Center").css("max-width", "1500px");			
			$("#Content").animate({"max-width": "1150px"});	
		}
	});
	

	//if browser window to small, hide #left
	
	if($( window ).width() < minWidthForLeft){
		$("#left").hide();
		$("#Content").css("margin-left", "0");
		$(".mainMenuB").show();
		$(".removeHome").hide();
		$("#networkMobile").show();			
		$("#networkMobileSmall").show();
		if($( window ).height() < minHeightMenu){
			$("#networkMobileSmall").show();
			$("#networkMobile").hide();					
		}
		else{
			$("#networkMobile").show();	
			$("#networkMobileSmall").hide();				
		}
	}

	$(window).resize(function() {
		if($( window ).width() < minWidthForLeft){
			$("#left").hide();
			$("#Content").css("margin-left", "0");
			$(".mainMenuB").show();
			$(".removeHome").hide();
			if($( window ).height() < minHeightMenu){
				$("#networkMobileSmall").show();
				$("#networkMobile").hide();					
			}
			else{
				$("#networkMobile").show();	
				$("#networkMobileSmall").hide();				
			}
		}
		else if($("#left").is(":hidden")){
			$("#left").show();
			$("#Content").css("margin-left", "350px");
			$(".mainMenuB").hide();			
			$(".removeHome").show();					
			$("#networkMobile").hide();			
			$("#networkMobileSmall").hide();			
		}
	});

	//if browser window height too small, 	#networkCenter should have top:150px, so it doesn't interfere with the logo
	//										and menus should be more narrow 
	
	if($( window ).height() < minHeightMenu){
		$("#networkCenter").css("top", "360px");		
		$(".networkElements").css("margin-bottom", "0");
		$("#Header").hide();
		$("#HeaderSmall").show();
		$("#Logo").hide();
		$("#LogoSmall").show();
	}

	$(window).resize(function() {
		if($(window ).height() < minHeightMenu){
			$("#networkCenter").css("top", "360px");		
			$(".networkElements").css("margin-bottom", "0");
			$("#Header").hide();
			$("#HeaderSmall").show();
			$("#Logo").hide();
			$("#LogoSmall").show();	
		}
		else if ($("#networkCenter").css("top")=="360px" && $(window ).height() >= minHeightMenu){
			$("#networkCenter").css({"top": "50%"});
			$(".networkElements").css("margin-bottom", "5%");
			$("#Header").show();
			$("#HeaderSmall").hide();
			$("#Logo").show();
			$("#LogoSmall").hide();	
		}
	});

	//if browser window height too small to actually show the network center, make it absolute
	if($( window ).height() < minHeightNetworkCenter){
		$("#networkCenter").css("position", "absolute");
	}
	$(window).resize(function() {
		if($( window ).height() < minHeightNetworkCenter){
			$("#networkCenter").css("position", "absolute");
		}
		else if($("#networkCenter").css("position")=="absolute" && $(window ).height() >= minHeightNetworkCenter){
			$("#networkCenter").css("position", "fixed");
		}
	});
		
	//in older browsers no shadows supported, replace with background
	if (document.createElement("detect").style.boxShadow != "") {
		$("html, body").css("background", "#eeeeee");
	}
	
	//if placeholders in forms are not supported, show descriptions 
	if(('placeholder' in document.createElement('input'))){
		$(".inputDescription").hide();
	}
	
	//on iPad adjust font size
	if(isiPad){
		$(".text").css("-webkit-text-size-adjust", "100%");
		$(".News").css("-webkit-text-size-adjust", "100%");		
	}
	
});

