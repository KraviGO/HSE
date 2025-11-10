import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App.jsx";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import CssBaseline from "@mui/material/CssBaseline";
import { UserProvider } from "./state/UserContext.jsx";

const qc = new QueryClient();

ReactDOM.createRoot(document.getElementById("root")).render(
    <React.StrictMode>
        <CssBaseline />
        <QueryClientProvider client={qc}>
            <UserProvider>
                <App />
            </UserProvider>
        </QueryClientProvider>
    </React.StrictMode>
);