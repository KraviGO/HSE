import { createContext, useContext, useEffect, useState } from "react";

const UserContext = createContext(null);

export function UserProvider({ children }) {
    const [user, setUser] = useState(() => {
        const raw = localStorage.getItem("currentUser");
        return raw ? JSON.parse(raw) : null;
    });

    useEffect(() => {
        if (user) localStorage.setItem("currentUser", JSON.stringify(user));
        else localStorage.removeItem("currentUser");
    }, [user]);

    return (
        <UserContext.Provider value={{ user, setUser }}>
            {children}
        </UserContext.Provider>
    );
}

export function useCurrentUser() {
    return useContext(UserContext);
}