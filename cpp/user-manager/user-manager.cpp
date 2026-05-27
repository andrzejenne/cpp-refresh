#include <algorithm>
#include <expected>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

struct User {
    int id;
    std::string name;
    std::string email;
};

std::ostream& operator<<(std::ostream& os, const User& user) {
    return os << user.name << " <" << user.email << "> #" << user.id;
}

bool isValidEmail(const std::string& email) {
    return email.contains('@') && email.contains('.');
}

const User* findUserById(const std::vector<User>& users, int id) {
    auto it = std::ranges::find_if(users, [id](const User& user) {
        return user.id == id;
    });

    if (it == users.end()) {
        return nullptr;
    }

    return std::addressof(*it);
}

std::expected<User, std::string> createUser(
    int id,
    std::string name,
    std::string email
) {
    if (id <= 0) {
        return std::unexpected("User id must be positive");
    }

    if (name.empty()) {
        return std::unexpected("Name must not be empty");
    }

    if (!isValidEmail(email)) {
        return std::unexpected("Email is not valid");
    }

    return User{
        .id = id,
        .name = std::move(name),
        .email = std::move(email),
    };
}

int main() {
    std::vector<User> users;

    auto addUser = [&users](int id, std::string name, std::string email) {
        auto created = createUser(id, std::move(name), std::move(email));

        if (!created) {
            std::cout << "Error: " << created.error() << "\n";
            return;
        }

        users.push_back(std::move(*created));
    };

    addUser(1, "Andrzej", "andrzej@example.com");
    addUser(2, "Ada", "ada@example.com");
    addUser(3, "Bjarne", "bjarne@example.com");
    addUser(-1, "Broken", "broken@example.com");
    addUser(4, "NoEmail", "invalid-email");

    if (auto user = findUserById(users, 1)) {
        std::cout << *user << ">\n";
    }

    auto usersWithExampleEmail = users | std::views::filter([](const User& user) {
        return user.email.ends_with("@example.com");
    });

    for (const auto& user : usersWithExampleEmail) {
        std::cout << user << "\n";
    }
}
