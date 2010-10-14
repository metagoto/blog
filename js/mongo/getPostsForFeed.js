function (limit) {

    var limit = limit || 10;

    var r = [];

    var a = db.post.find( { } )
            .sort( { date: -1 } )
            .limit(limit);

    for (var i = 0, x; x = a[i]; ++i) {

        delete x.coms;

        r.push(x);
    }
    return r;
}
