import { BrowserRouter, Routes, Route, Navigate } from "react-router-dom";
import NavBar from "./components/NavBar";
import UsersPage from "./pages/UsersPage";
import AccountsPage from "./pages/AccountsPage";
import CategoriesPage from "./pages/CategoriesPage";
import OperationsPage from "./pages/OperationsPage";
import TransfersPage from "./pages/TransfersPage";
import GoalsPage from "./pages/GoalsPage";
import AnalyticsPage from "./pages/AnalyticsPage";
import ImportExportPage from "./pages/ImportExportPage";
import Container from "@mui/material/Container";

export default function App() {
    return (
        <BrowserRouter>
            <NavBar />
            <Container sx={{ my: 3 }}>
                <Routes>
                    <Route path="/" element={<UsersPage />} />
                    <Route path="/accounts" element={<AccountsPage />} />
                    <Route path="/categories" element={<CategoriesPage />} />
                    <Route path="/operations" element={<OperationsPage />} />
                    <Route path="/transfers" element={<TransfersPage />} />
                    <Route path="/goals" element={<GoalsPage />} />
                    <Route path="/analytics" element={<AnalyticsPage />} />
                    <Route path="/import-export" element={<ImportExportPage />} />
                    <Route path="*" element={<Navigate to="/" replace />} />
                </Routes>
            </Container>
        </BrowserRouter>
    );
}