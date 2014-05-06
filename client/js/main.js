$(function() {
	check_messages = function() {
		
		function getcookie(name) {
			var cookie = " " + document.cookie;
			var search = " " + name + "=";
			var setStr = null;
			var offset = 0;
			var end = 0;
			if (cookie.length > 0) {
				offset = cookie.indexOf(search);
				if (offset != -1) {
					offset += search.length;
					end = cookie.indexOf(";", offset)
					if (end == -1) {
						end = cookie.length;
					}
					setStr = unescape(cookie.substring(offset, end));
				}
			}
			return(setStr);
		}
		
		$.ajax({
			url : "?action=get_md5",
			success : function(new_md5) {
				var cur_md5 = getcookie("messages_md5");
				if (cur_md5 != new_md5) {
					document.location.reload();
				}
			}
		});
		
		setTimeout(function() {
			check_messages();
		}, 50);
		
		$('input:text:first').focus();
	};
	check_messages();
});
