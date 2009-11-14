function (id, skip, limit) {

    var skip = skip || 0;
    var limit = limit || 10;

    var r = [];

    var a = db.post.find( { _cats: id } )
            .sort( { date: -1 } )
            .skip(skip)
            .limit(limit);

    for (var i = 0, x; x = a[i]; ++i) {
        fillPostMeta(x);
        delete x._tags;
        delete x._cats;

        x.numComs = x.coms.length;
        delete x.coms;

        r.push(x);
    }
    return r;
}
