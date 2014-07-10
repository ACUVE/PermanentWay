#ifndef ACUVE_SCOPE_EXIT_HPP
#define ACUVE_SCOPE_EXIT_HPP

#include <utility>

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) a ## b

template<typename F>
struct scope_exit_t {
    scope_exit_t(F & f) : f(f) {}
    ~scope_exit_t() { f(); }
private:
    F & f;
};

template<typename F>
struct scope_exit_t<F&&> {
    scope_exit_t(F && f) : f(f) {}
    ~scope_exit_t() { f(); }
private:
    F f;
};

struct scope_exit_helper {
    template<typename F>
    scope_exit_t<F&&> operator->*(F && f) const {
        return scope_exit_t<F&&>(std::forward<F>(f));
    }
};

#define scope_exit_base auto const PP_CAT(scope_exit_, __LINE__) = scope_exit_helper() ->*
#define scope_exit scope_exit_base [&] ()
#define scope_exit_2 scope_exit_base

#endif // ACUVE_SCOPE_EXIT_HPP
