// * url = "http://github.com/carbonfive/raygun" -> domain name = "github"


function domainName(url) {
    // Удаление протокола
    url = url.replace(/https?:\/\//, '');
    // Удаление 'www.'
    url = url.replace('www.', '');
    // Возврат доменного имени до первого '/'
    return url.split('.')[0];
}
