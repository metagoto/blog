function () {

    var cats = [];
    db.cat.find().forEach(function (x) {
        cats[x._id] = x.name;
    });

    var tmp = [];
    db.post.find( {}, { _cats: 1 }).forEach(function (x) {
        for (var i = 0, t; t = x._cats[i]; ++i) {
            if (cats[t]) {
                tmp[t] = { _id: t, name: cats[t], count: 1 + (tmp[t] ? tmp[t].count : 0) };
            }
        }
    });

    var cloud = [];
    for (var t in tmp) {
        cloud.push(tmp[t]);
    }

    return cloud.sort(function (l, r) { return l.name.toLowerCase() > r.name.toLowerCase() });
}
