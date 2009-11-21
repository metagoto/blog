function () {

    var tags = [];
    db.tag.find().forEach(function (x) {
        tags[x._id] = x.name;
    });

    var tmp = [];
    db.post.find( {}, { _tags: 1 }).forEach(function (x) {
        for (var i = 0, t; t = x._tags[i]; ++i) {
            if (tags[t]) {
                tmp[t] = { _id: t, name: tags[t], count: 1 + (tmp[t] ? tmp[t].count : 0) };
            }
        }
    });

    var cloud = [];
    for (var t in tmp) {
        cloud.push(tmp[t]);
    }

    return cloud.sort(function (l, r) { return l.name.toLowerCase() > r.name.toLowerCase() });
}
