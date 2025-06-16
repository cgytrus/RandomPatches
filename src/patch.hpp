#pragma once

template <class OkType>
void logErr(const std::string_view& path, const Result<OkType>&& res) {
    if (res)
        return;
    auto lastSlash = std::max(path.rfind('/'), path.rfind('\\'));
    if (lastSlash == std::string_view::npos)
        lastSlash = 0;
    auto lastDot = path.rfind('.');
    if (lastDot == std::string_view::npos)
        lastDot = path.size();
    const std::string_view file = path.substr(lastSlash, lastDot - lastSlash);
    log::error("{} patch/hook failed: {}", file, res.unwrapErr());
}
#define LOG_ERR(...) logErr(__FILE__, __VA_ARGS__)
