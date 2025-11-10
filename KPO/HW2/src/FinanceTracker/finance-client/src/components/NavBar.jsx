import React from "react";
import { AppBar, Toolbar, Typography, Button, Stack } from "@mui/material";
import { Link, useLocation } from "react-router-dom";

const items = [
    { to: "/", label: "👤 Пользователи" },
    { to: "/accounts", label: "💳 Счета" },
    { to: "/operations", label: "💸 Операции" },
    { to: "/categories", label: "🏷 Категории" },
    { to: "/transfers", label: "🔁 Переводы" },
    { to: "/goals", label: "🎯 Цели" },
    { to: "/analytics", label: "📊 Аналитика" },
    { to: "/import-export", label: "📂 Импорт/Экспорт" },
];

export default function NavBar() {
    const { pathname } = useLocation();

    return (
        <AppBar position="static">
            <Toolbar sx={{ gap: 2 }}>
                <Typography variant="h6" sx={{ flexGrow: 1 }}>
                    Finance Tracker
                </Typography>
                <Stack direction="row" spacing={1}>
                    {items.map((it) => (
                        <Button
                            key={it.to}
                            color={pathname === it.to ? "secondary" : "inherit"}
                            component={Link}
                            to={it.to}
                        >
                            {it.label}
                        </Button>
                    ))}
                </Stack>
            </Toolbar>
        </AppBar>
    );
}